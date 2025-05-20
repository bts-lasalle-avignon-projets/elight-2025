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
        qWarning() << Q_FUNC_INFO
                   << " Erreur bind socket UDP" + QString::number(portSocket);
        return;
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
    qDebug() << Q_FUNC_INFO << "datagramme" << datagramme.trimmed() << "adresse"
             << adresse.toString() << "port" << port;

    if(!datagramme.startsWith(DELIMITEUR_DEBUT) ||
       !datagramme.endsWith(DELIMITEUR_FIN))
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

    int     idSegment = recupererIdSegment(adresse.toString());
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
                                  adresse.toString(),
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
                                  adresse.toString(),
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
    int   idSegment = recupererIdSegment(adresseIPSegment);
    float puissance = donnees.toFloat();

    qDebug() << Q_FUNC_INFO << "adresseIPSegment" << adresseIPSegment
             << "idSegment" << idSegment
             << "puissance" + QString::number(puissance);

    emit puissanceInstantaneeSegmentRecue(idSegment, puissance);
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
