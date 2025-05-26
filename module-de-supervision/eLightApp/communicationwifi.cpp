#include "communicationwifi.h"
#include <QHostAddress>
#include <QMap>
#include <QFile>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QCoreApplication>
#include <QDebug>

CommunicationWiFi::CommunicationWiFi(QObject* parent) :
    QObject(parent), udpSocket(nullptr), portSocket(PORT_DEFAUT),
    rafraichissement(RAFRAICHISSEMENT_DEFAUT)
{
    qDebug() << Q_FUNC_INFO << this << "parent" << parent;
    chargerConfiguration();
    initialiserSocket();
}

CommunicationWiFi::~CommunicationWiFi()
{
    if(udpSocket)
    {
        udpSocket->close();
        delete udpSocket;
    }
    qDebug() << Q_FUNC_INFO << this;
}

void CommunicationWiFi::chargerConfiguration()
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config-elightapp.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);

        portSocket =
          parametres.value("CommunicationWiFi/port", portSocket).toInt();
        rafraichissement =
          parametres
            .value("CommunicationWiFi/rafraichissement", rafraichissement)
            .toInt();

        qDebug() << Q_FUNC_INFO << "portSocket" << portSocket
                 << "rafraichissement" << rafraichissement;
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Fichier de configuration non trouvé"
                   << cheminConfiguration;
    }
}

void CommunicationWiFi::initialiserSocket()
{
    qDebug() << Q_FUNC_INFO;

    udpSocket = new QUdpSocket(this);

    if(!udpSocket->bind(QHostAddress::Any, portSocket))
    {
        qWarning() << Q_FUNC_INFO << "Erreur bind sur port" << portSocket << ":"
                   << udpSocket->errorString();

        // Tentative automatique avec un autre port (facultatif mais utile pour
        // test)
        quint16 portFallback = 5500;
        if(portSocket != portFallback)
        {
            qWarning() << Q_FUNC_INFO << "Tentative sur port de secours"
                       << portFallback;

            if(!udpSocket->bind(QHostAddress::Any, portFallback))
            {
                qWarning() << Q_FUNC_INFO << "Échec aussi sur port de secours"
                           << ":" << udpSocket->errorString();
                return;
            }
            else
            {
                qInfo() << Q_FUNC_INFO << "Bind réussi sur port de secours"
                        << portFallback;
                portSocket = portFallback; // Met à jour la valeur utilisée
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        qInfo() << Q_FUNC_INFO << "Bind réussi sur port" << portSocket;
    }

    connect(udpSocket,
            &QUdpSocket::readyRead,
            this,
            &CommunicationWiFi::lireDatagramme);
}

void CommunicationWiFi::lireDatagramme()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagramme;
        datagramme.resize(int(udpSocket->pendingDatagramSize()));
        QHostAddress adresse;
        quint16      port;

        udpSocket->readDatagram(datagramme.data(),
                                datagramme.size(),
                                &adresse,
                                &port);
        QString datagrammeRecu = QString::fromUtf8(datagramme);

        qDebug() << Q_FUNC_INFO << "datagramme" << datagramme.trimmed()
                 << "adresse" << adresse.toString() << "port" << port;

        traiterDatagramme(datagrammeRecu, adresse, port);
    }
}

void CommunicationWiFi::envoyerDatagramme(const QString& type,
                                          const QString& donnees,
                                          const QString& adresse,
                                          quint16        port)
{
    QString    datagramme = QString("#%1;%2\r\n").arg(type, donnees);
    QByteArray data       = datagramme.toUtf8();
    udpSocket->writeDatagram(data, QHostAddress(adresse), port);

    qDebug() << Q_FUNC_INFO << "datagramme" << datagramme.trimmed() << "adresse"
             << adresse << "port" << port;
}

void CommunicationWiFi::traiterDatagramme(const QString&      datagramme,
                                          const QHostAddress& adresse,
                                          quint16             port)
{
    QString adresseReglee = adresse.toString();
    if(adresseReglee.startsWith("::ffff:"))
        adresseReglee = adresseReglee.right(adresseReglee.length() - 7);

    qDebug() << Q_FUNC_INFO << "datagramme" << datagramme.trimmed() << "adresse"
             << adresseReglee << "port" << port;

    if(!datagramme.startsWith(DELIMITEUR_DEBUT))
    {
        qWarning() << Q_FUNC_INFO << "Format invalide !";
        return;
    }

    // Supprime les délimiteurs
    QString contenu = datagramme.mid(1, datagramme.length() - 3);
    // Décomposition du datagramme
    QStringList champs = contenu.split(";");

    if(champs.size() != NB_CHAMPS && champs.size() != NB_CHAMPS_CONFIGURATION)
    {
        qWarning() << Q_FUNC_INFO << "Nb champs invalide !" << champs.size();
        return;
    }

    int     idSegment = recupererIdSegment(adresseReglee, idSegment);
    QString type      = champs[CHAMP_TYPE];
    QString donnees   = champs[CHAMP_DONNEE]; // par défaut une seule donnée
    bool    ok        = false;
    int     puissance;

    switch(type.at(0).toLatin1())
    {
        case 'A': // Acquittement
            if(donnees == "0")
            {
                emit acquittementRecue(idSegment);
            }
            break;
        case 'S': // Signalement d'un segement d'une salle
            if(champs.size() == NB_CHAMPS_CONFIGURATION)
            {
                qDebug() << Q_FUNC_INFO << "NOM_SALLE"
                         << champs[CHAMP_NOM_SALLE] << "NUMERO_SEGMENT"
                         << champs[CHAMP_NUMERO_SEGMENT] << "NB_SEGMENTS"
                         << champs[CHAMP_NB_SEGMENTS] << "SUPERFICIE_SALLE"
                         << champs[CHAMP_SUPERFICIE_SALLE];
                emit configurationRecue(adresse.toString(),
                                        champs[CHAMP_NOM_SALLE],
                                        champs[CHAMP_NUMERO_SEGMENT],
                                        champs[CHAMP_NB_SEGMENTS],
                                        champs[CHAMP_SUPERFICIE_SALLE]);
                envoyerDatagramme("A",
                                  "0",
                                  adresseReglee,
                                  port); // Acquittement
            }
            break;
        case 'P': // Puissance
            puissance = donnees.toInt(&ok);
            if(ok)
            {
                qDebug() << Q_FUNC_INFO << "idSegment" << idSegment
                         << "puissance" << puissance << "W";
                envoyerDatagramme("A",
                                  "0",
                                  adresseReglee,
                                  port); // Acquittement
                emit puissanceInstantaneeSegmentRecue(idSegment, puissance);
            }
            else
            {
                qWarning() << Q_FUNC_INFO << "Puissance invalide !";
            }
            break;
        default:
            qWarning() << Q_FUNC_INFO << "Type inconnu !";
    }
}

void CommunicationWiFi::traiterPuissanceTrame(const QString& adresseIPSegment,
                                              const QString& donnees)
{
    int idSegment;

    recupererIdSegment(adresseIPSegment, idSegment);

    float puissance = donnees.toFloat();

    emit puissanceInstantaneeSegmentRecue(idSegment, puissance);

    qDebug() << Q_FUNC_INFO << "adresseIPSegment" << adresseIPSegment
             << "idSegment" << idSegment
             << "puissance" + QString::number(puissance);
}

int CommunicationWiFi::recupererIdSegment(const QString& adresseIPSegment,
                                          int&           idSegment)
{
    QSqlQuery requete;
    idSegment = SEGMENT_INDEFINI;

    requete.prepare(
      "SELECT id_segment FROM segment WHERE ip_segment = :ip_segment");

    requete.bindValue(":ip_segment", adresseIPSegment);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
    }
    else
    {
        if(requete.next())
        {
            idSegment = requete.value(0).toInt();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << " zero résultat " << adresseIPSegment;
        }
    }

    qDebug() << Q_FUNC_INFO << "adresseIPSegment" << adresseIPSegment
             << "idSegment" << idSegment;

    return idSegment;
}

bool CommunicationWiFi::recupererPort(int& port)
{
    QString cheminConfiguration =
      QCoreApplication::applicationDirPath() + "/config-elightapp.ini";

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);
        port = parametres.value("CommunicationWifi/port").toInt();

        return true;
    }
    else
    {
        return false;
    }
}

void CommunicationWiFi::envoyerTrameDemandePuissance(const QString& adresse)
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
            qDebug() << Q_FUNC_INFO << "Adresse " << adresse << " Trame "
                     << trame.trimmed();
        }
    }
    else
    {
        qWarning() << Q_FUNC_INFO << " Port non trouvé ";
    }
}

void CommunicationWiFi::envoyerTrameIntensite(const QString& adresse,
                                              const int&     intensite)
{
    QString    trame   = "#I;" + QString::number(intensite) + "\r\n";
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
            qDebug() << Q_FUNC_INFO << "Adresse " << adresse << " Trame "
                     << trame.trimmed();
        }
    }
    else
    {
        qWarning() << Q_FUNC_INFO << " Port non trouvé ";
    }
}

QString CommunicationWiFi::recupererAdresseDestination(const int& idSegment)
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

/*bool CommunicationWiFi::recupererNomSalle(QString& adresse)
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

QStringList CommunicationWiFi::recupererAdressesDestinations()
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
}*/
