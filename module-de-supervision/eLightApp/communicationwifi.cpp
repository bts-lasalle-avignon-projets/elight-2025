#include "communicationwifi.h"
#include <QDebug>

communicationWiFi::communicationWiFi(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << this << "parent" << parent;
    initialiserSocket();
}

communicationWiFi::~communicationWiFi()
{
    if(udpSocket)
    {
        udpSocket->close();
        delete udpSocket;
    }
}

void communicationWiFi::initialiserSocket()
{
    udpSocket = new QUdpSocket(this);

    QSettings settings("config-base-de-donnees.ini", QSettings::IniFormat);

    portSocket = settings.value("CommunicationWiFi/port", portSocket).toInt();

    if(!udpSocket->bind(QHostAddress::Any, portSocket))
    {
        qWarning() << Q_FUNC_INFO
                   << " Erreur binding UDP socket:" +
                        QString::number(portSocket);
        return;
    }
    else
    {
        qDebug() << Q_FUNC_INFO << " Connexion " + QString::number(portSocket);
    }

    connect(udpSocket,
            &QUdpSocket::readyRead,
            this,
            &communicationWiFi::lireTrame);
}

void communicationWiFi::lireTrame()
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(int(udpSocket->pendingDatagramSize()));
        QHostAddress sender;
        quint16      senderPort;

        udpSocket->readDatagram(datagram.data(),
                                datagram.size(),
                                &sender,
                                &senderPort);
        QString trame = QString::fromUtf8(datagram).trimmed();

        qDebug() << "Trame reçue:" << trame << "de" << sender.toString() << ":"
                 << senderPort;

        traiterTrame(trame, sender, senderPort);
    }
}

void communicationWiFi::envoyerTrame(const QString&      type,
                                     const QString&      donnees,
                                     const QHostAddress& adresse,
                                     quint16             port)
{
    QString    trame = QString("#%1;%2\r\n").arg(type, donnees);
    QByteArray data  = trame.toUtf8();
    udpSocket->writeDatagram(data, adresse, port);
    qDebug() << "Trame envoyée:" << trame.trimmed() << "à" << adresse.toString()
             << ":" << port;
}

void communicationWiFi::traiterTrame(const QString&      trame,
                                     const QHostAddress& sender,
                                     quint16             senderPort)
{
    if(!trame.startsWith("#") || !trame.endsWith("\r\n"))
    {
        qWarning() << "Mauvaise syntaxe de trame:" << trame;
        return;
    }

    QString     contenu = trame.mid(1, trame.length() - 3);
    QStringList parties = contenu.split(";");

    if(parties.size() != 2)
    {
        qWarning() << "Trame invalide:" << trame;
        return;
    }

    QString type    = parties[0];
    QString donnees = parties[1];

    QString ipSource  = sender.toString();
    int     idSegment = -1;
    recupererIdSegment(ipSource, idSegment);

    if(type == "P")
    {
        bool ok        = false;
        int  puissance = donnees.toInt(&ok);
        if(ok)
        {
            qDebug() << "Puissance instantanée reçue du segment" << idSegment
                     << ":" << puissance << "W";
            traiterPuissanceTrame(idSegment, puissance);

            envoyerTrame("A", "0", sender, senderPort);
        }
        else
        {
            qWarning() << "Puissance invalide:" << donnees;
        }
    }
    else if(type == "A" && donnees == "0")
    {
        qDebug() << "Acquittement reçu du segment" << idSegment;
    }
    else if(type == "I")
    {
        bool ok        = false;
        int  intensite = donnees.toInt(&ok);
        if(ok)
        {
            qDebug() << "Intensité reçue du segment" << idSegment << ":"
                     << intensite << "lux";
            emit intensiteSegmentRecue(idSegment, intensite);
            envoyerTrame("A", "0", sender, senderPort);
        }
    }
    else
    {
        qWarning() << "Type de trame inconnu:" << type;
    }
}

void communicationWiFi::traiterPuissanceTrame(const QString& ipSource,
                                              const QString& donnees)
{
    int idSegment;

    recupererIdSegment(ipSource, idSegment);

    float puissance = donnees.toFloat();

    emit puissanceInstantaneeSegmentRecue(idSegment, puissance);

    qDebug() << "Source : " + ipSource + " idSegment : " + idSegment +
                  " puissance : " + QString::number(puissance);
}

void communicationWiFi::recupererIdSegment(QString adresseSourceReglee,
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
            qDebug() << Q_FUNC_INFO << " aucun résultat "
                     << adresseSourceReglee;
        }
    }
}
