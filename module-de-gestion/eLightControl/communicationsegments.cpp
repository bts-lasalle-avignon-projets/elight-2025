#include "communicationsegments.h"
#include <QDebug>

CommunicationSegments::CommunicationSegments(QObject* parent) :
    QObject(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    qDebug() << Q_FUNC_INFO << this << "parent" << parent;

    udpSocketEmission = new QUdpSocket(this);
    timerEmission     = new QTimer(this);

    connect(timerEmission, &QTimer::timeout, this, [this]() {
        QStringList adresses = recupererAdressesDestinations();
        for(const QString& adresse: adresses)
        {
            envoyerTrameUDP(adresse);
        }
    });

    if(initialiserTempsRafraichissement())
    {
        timerEmission->start(getTempsRafraichissement());
    }
    else
    {
        qWarning() << Q_FUNC_INFO << " temps_rafraichissement non trouvé";
    }

    initialiserSocket();
}

CommunicationSegments::~CommunicationSegments()
{
    if(udpSocket)
    {
        udpSocket->close();
        delete udpSocket;
    }
}

void CommunicationSegments::initialiserSocket()
{
    udpSocket = new QUdpSocket(this);

    QSettings settings("config.ini", QSettings::IniFormat);

    portWebSocket =
      settings.value("CommunicationSegment/port", portWebSocket).toInt();

    if(!udpSocket->bind(QHostAddress::Any, portWebSocket))
    {
        qWarning() << Q_FUNC_INFO
                   << " Erreur binding " + QString::number(portWebSocket);
        return;
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << " Connexion " + QString::number(portWebSocket);
    }

    connect(udpSocket,
            &QUdpSocket::readyRead,
            this,
            &CommunicationSegments::traiterTrameRecue);
}

void CommunicationSegments::traiterTrameRecue()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray donneesRecues;
        donneesRecues.resize(
          static_cast<int>(udpSocket->pendingDatagramSize()));

        QHostAddress adresseSource;

        udpSocket->readDatagram(donneesRecues.data(),
                                donneesRecues.size(),
                                &adresseSource);

        QString trame = QString::fromUtf8(donneesRecues).trimmed();

        if(trame.startsWith("#"))
        {
            QString puissanceInstantanee;
            QString nomSalle;
            QString numeroSegment;
            QString nombreSegments;
            QString superficieSalle;

            trame.remove(0, 1);

            QStringList elements = trame.split(";");

            if(elements.size() >= ELEMENTS_MIN_TRAME &&
               elements.size() <= ELEMENTS_MAX_TRAME)
            {
                QString type = elements[TYPE_TRAME].trimmed();

                if(elements.size() == ELEMENTS_MIN_TRAME)
                {
                    puissanceInstantanee =
                      elements[PUISSANCE_INSTANTANEE_TRAME].trimmed();
                }
                else if(elements.size() == ELEMENTS_MAX_TRAME)
                {
                    nomSalle        = elements[NOM_SALLE_TRAME].trimmed();
                    numeroSegment   = elements[NUMERO_SEGMENT_TRAME].trimmed();
                    nombreSegments  = elements[NOMBRE_SEGMENTS_TRAME].trimmed();
                    superficieSalle = elements[SUPERFICIE_SALLE].trimmed();
                }
                else
                {
                    qWarning() << Q_FUNC_INFO << " Trame invalide" << trame;
                    return;
                }

                QString adresseSourceReglee = adresseSource.toString();

                if(adresseSourceReglee.startsWith("::ffff:"))
                    adresseSourceReglee = adresseSourceReglee.mid(7);

                if(type == "P")
                {
                    traiterTramePuissance(adresseSourceReglee,
                                          puissanceInstantanee);
                }

                else if(type == "S")
                {
                    qDebug() << Q_FUNC_INFO << " nomSalle " << nomSalle
                             << " numeroSegment " << numeroSegment
                             << " nombreSegments " << nombreSegments
                             << " superficieSalle " << superficieSalle;
                }
            }

            else
            {
                qWarning() << Q_FUNC_INFO << " Trame invalide" << trame;
                return;
            }
        }
    }
}

void CommunicationSegments::traiterTramePuissance(const QString& ipSource,
                                                  const QString& donnees)
{
    int idSegment;

    recupererIdSegment(ipSource, idSegment);

    float puissance = donnees.toFloat();

    emit puissanceInstantaneeSegmentRecue(idSegment, puissance);

    qDebug() << "Source : " + ipSource + " idSegment : " + idSegment +
                  " puissance : " + QString::number(puissance);
}

void CommunicationSegments::recupererIdSegment(QString adresseSourceReglee,
                                               int&    idSegment)
{
    QSqlQuery requete;

    requete.prepare(
      "SELECT id_segment FROM segment WHERE ip_segment = :ip_segment");

    requete.bindValue(":ip_segment", adresseSourceReglee);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
    }
    else
    {
        if(requete.next())
        {
            idSegment = requete.value(0).toInt();
            qDebug() << Q_FUNC_INFO << " idSegment " << idSegment;
        }
        else
        {
            qDebug() << Q_FUNC_INFO << " zero résultat " << adresseSourceReglee;
        }
    }
}

bool CommunicationSegments::recupererPort(int& port)
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);
        port = parametres.value("CommunicationSegment/port").toInt();

        return true;
    }
    else
    {
        return false;
    }
}

void CommunicationSegments::envoyerTrameUDP(const QString& adresse)
{
    QString    trame   = "#P;0\r\n";
    QByteArray donnees = trame.toUtf8();

    QHostAddress adresseDestinataire(adresse);

    int port;

    if(recupererPort(port))
    {
        quint16 portDestinataire = port;

        qint64 octetsEnvoyes =
          udpSocketEmission->writeDatagram(donnees,
                                           adresseDestinataire,
                                           portDestinataire);

        if(octetsEnvoyes == -1)
        {
            qWarning() << Q_FUNC_INFO << " Erreur envoi "
                       << udpSocketEmission->errorString();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << " Trame " << trame.trimmed();
        }
    }
    else
    {
        qWarning() << Q_FUNC_INFO << " Port non trouvé ";
    }
}

bool CommunicationSegments::initialiserTempsRafraichissement()
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);
        tempsRafraichissement =
          parametres.value("CommunicationSegment/temps_rafraichissement")
            .toInt();

        return true;
    }
    else
    {
        return false;
    }
}

int CommunicationSegments::getTempsRafraichissement()
{
    return tempsRafraichissement;
}

QString CommunicationSegments::recupererAdresseDestination(const int& idSegment)
{
    QSqlQuery requete;
    QString   adresse;

    requete.prepare(
      "SELECT ip_segment FROM segment WHERE id_segment = :id_segment");

    requete.bindValue(":id_segment", idSegment);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
    }
    else
    {
        if(requete.next())
        {
            adresse = requete.value(0).toString();
            qDebug() << Q_FUNC_INFO << " ipSegment " << adresse;
        }
        else
        {
            qDebug() << Q_FUNC_INFO << " zero résultat " << idSegment;
        }
    }
    return adresse;
}

bool CommunicationSegments::recupererNomSalle(QString& adresse)
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);
        adresse = parametres.value("Salle/nom").toString();

        return true;
    }
    else
    {
        return false;
    }
}

QStringList CommunicationSegments::recupererAdressesDestinations()
{
    QStringList adresses;
    QSqlQuery   requete;
    QString     salle;

    if(recupererNomSalle(salle))
    {
        requete.prepare(
          "SELECT ip_segment FROM segment WHERE id_salle = "
          "(SELECT id_salle FROM salle WHERE nom_salle = :nomSalle)");

        requete.bindValue(":nomSalle", salle);

        if(!requete.exec())
        {
            qDebug() << Q_FUNC_INFO << "Erreur SQL"
                     << requete.lastError().text();
        }
        else
        {
            while(requete.next())
            {
                adresses << requete.value(0).toString();
            }
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "Erreur SQL : " << requete.lastError().text();
    }
    return adresses;
}
