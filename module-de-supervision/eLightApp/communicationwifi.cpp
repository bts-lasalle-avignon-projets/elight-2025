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
    qDebug() << Q_FUNC_INFO << "portSocket" << portSocket;

    udpSocket = new QUdpSocket(this);

    if(!udpSocket->bind(QHostAddress::Any, portSocket))
    {
        qWarning() << Q_FUNC_INFO << "Erreur bind sur port" << portSocket << ":"
                   << udpSocket->errorString();

        return;
    }

    connect(udpSocket,
            &QUdpSocket::readyRead,
            this,
            &CommunicationWiFi::lireDatagramme);

    connect(this,
            &CommunicationWiFi::puissanceInstantaneeSegmentRecue,
            this,
            &CommunicationWiFi::traiterPuissance);

    connect(this,
            &CommunicationWiFi::acquittementRecue,
            this,
            &CommunicationWiFi::traiterAcquittement);

    connect(this,
            &CommunicationWiFi::configurationRecue,
            this,
            &CommunicationWiFi::traiterConfiguration);
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
    QString contenu = datagramme;
    contenu         = contenu.trimmed();
    contenu         = contenu.mid(1, contenu.length() - 1);

    // Décomposition du datagramme
    QStringList champs = contenu.split(";");

    if(champs.size() != NB_CHAMPS && champs.size() != NB_CHAMPS_CONFIGURATION)
    {
        qWarning() << Q_FUNC_INFO << "Nb champs invalide !" << champs.size();
        return;
    }

    int     idSegment = recupererIdSegment(adresseReglee);
    QString type      = champs[CHAMP_TYPE];
    QString donnees   = champs[CHAMP_DONNEE]; // par défaut une seule donnée
    bool    ok        = false;
    int     puissance;

    switch(type.at(0).toLatin1())
    {
        case 'A': // Acquittement
            if(donnees == "0")
            {
                if(idSegment != SEGMENT_INDEFINI)
                    emit acquittementRecue(adresseReglee, idSegment);
            }
            break;
        case 'S': // Signalement d'un segment d'une salle
            if(champs.size() == NB_CHAMPS_CONFIGURATION)
            {
                qDebug() << Q_FUNC_INFO << "NOM_SALLE"
                         << champs[CHAMP_NOM_SALLE] << "NUMERO_SEGMENT"
                         << champs[CHAMP_NUMERO_SEGMENT] << "NB_SEGMENTS"
                         << champs[CHAMP_NB_SEGMENTS] << "SUPERFICIE_SALLE"
                         << champs[CHAMP_SUPERFICIE_SALLE];
                emit configurationRecue(adresseReglee,
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
        case 'P': // Puissance instantanée
            if(idSegment == SEGMENT_INDEFINI)
                return;

            donnees = donnees.trimmed();

            qDebug() << Q_FUNC_INFO
                     << "Valeur brute de la puissance (donnees):" << donnees;

            puissance = donnees.toInt(&ok);

            if(ok)
            {
                qDebug() << Q_FUNC_INFO << "idSegment" << idSegment
                         << "puissance" << puissance << "W";
                envoyerDatagramme("A",
                                  "0",
                                  adresseReglee,
                                  port); // Acquittement
                emit puissanceInstantaneeSegmentRecue(adresseReglee,
                                                      idSegment,
                                                      puissance);
            }
            else
            {
                qWarning() << Q_FUNC_INFO << "Puissance invalide !" << donnees;
            }
            break;
        default:
            qWarning() << Q_FUNC_INFO << "Type inconnu !";
    }
}

int CommunicationWiFi::recupererIdSegment(const QString& adresseIPSegment)
{
    QSqlQuery requete;
    int       idSegment = SEGMENT_INDEFINI;

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
    }

    qDebug() << Q_FUNC_INFO << "adresseIPSegment" << adresseIPSegment
             << "idSegment" << idSegment;

    return idSegment;
}

void CommunicationWiFi::envoyerTrameDemandePuissance(const QString& adresse,
                                                     quint16        port)
{
    QString    trame   = "#P;0\r\n";
    QByteArray donnees = trame.toUtf8();

    QHostAddress adresseDestinataire(adresse);

    qint64 octetsEnvoyes =
      udpSocketEmission->writeDatagram(donnees, adresseDestinataire, port);

    if(octetsEnvoyes == -1)
    {
        qWarning() << Q_FUNC_INFO << "Erreur envoi"
                   << udpSocketEmission->errorString();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "adresse" << adresse << "port" << port
                 << "trame" << trame.trimmed();
    }
}

void CommunicationWiFi::envoyerTrameIntensite(const int&     intensite,
                                              const QString& adresse,
                                              quint16        port)
{
    QString    trame   = "#I;" + QString::number(intensite) + "\r\n";
    QByteArray donnees = trame.toUtf8();

    QHostAddress adresseDestinataire(adresse);

    qint64 octetsEnvoyes =
      udpSocketEmission->writeDatagram(donnees, adresseDestinataire, port);

    if(octetsEnvoyes == -1)
    {
        qWarning() << Q_FUNC_INFO << " Erreur envoi "
                   << udpSocketEmission->errorString();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "adresse" << adresse << "port" << port
                 << "trame" << trame.trimmed();
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
            qDebug() << Q_FUNC_INFO << "adresse" << adresse;
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "aucun résultat !"
                     << "idSegment" << idSegment;
        }
    }

    return adresse;
}

void CommunicationWiFi::traiterPuissance(QString adresse,
                                         int     idSegment,
                                         int     puissance)
{
    qDebug() << Q_FUNC_INFO << "puissance" << puissance << "W" << adresse
             << adresse << "idSegment" << idSegment;

    QDateTime now = QDateTime::currentDateTimeUtc();

    double dureeHeures = 0;

    if(m_lastTimestampParSegment.contains(idSegment) &&
       m_lastTimestampParSegment[idSegment].isValid())
    {
        qint64 msecs = m_lastTimestampParSegment[idSegment].msecsTo(now);
        dureeHeures  = msecs / 1000.0 / 3600.0; // conversion ms -> heures
    }
    else
    {
        // Première mesure pour ce segment
        dureeHeures = 0;
    }

    m_lastTimestampParSegment[idSegment] = now;

    if(dureeHeures > 0)
    {
        double consommation = (puissance * dureeHeures) / 1000.0;

        qDebug() << Q_FUNC_INFO << "Segment" << idSegment
                 << "Intervalle (h):" << dureeHeures
                 << "Consommation calculée:" << consommation << "kWh";

        QSqlQuery requete;
        requete.prepare("INSERT INTO historique_consommation_segment "
                        "(id_segment, consommation) "
                        "VALUES (:id_segment, :consommation)");
        requete.bindValue(":id_segment", idSegment);
        requete.bindValue(":consommation", consommation);

        if(!requete.exec())
        {
            qWarning() << Q_FUNC_INFO << "Erreur SQL"
                       << requete.lastError().text();
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Segment" << idSegment
                 << "Première mesure reçue, pas de calcul de consommation";
    }
}

void CommunicationWiFi::traiterAcquittement(QString adresse, int idSegment)
{
    qDebug() << Q_FUNC_INFO << "adresse" << adresse << "idSegment" << idSegment;
}

void CommunicationWiFi::traiterConfiguration(QString adresse,
                                             QString nomSalle,
                                             QString numeroSegment,
                                             QString nbSegments,
                                             QString superficie)
{
    qDebug() << Q_FUNC_INFO << "adresse" << adresse << "nomSalle" << nomSalle
             << "numeroSegment" << numeroSegment << "nbSegments" << nbSegments
             << "superficie" << superficie;
    /**
     * @todo Gérer cette configuration de salle
     */
}
