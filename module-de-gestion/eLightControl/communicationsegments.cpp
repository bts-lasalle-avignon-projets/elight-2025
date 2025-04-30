#include "communicationsegments.h"
#include <QDebug>

CommunicationSegments::CommunicationSegments(QObject* parent) : QObject(parent)
{
    qDebug() << Q_FUNC_INFO << this << "parent" << parent;
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
                   << "Erreur de binding sur le port " +
                        QString::number(portWebSocket);
        return;
    }
    else
    {
        qDebug() << Q_FUNC_INFO
                 << "Connexion réussie sur le port " +
                      QString::number(portWebSocket);
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

        QString adresseSourceReglee = adresseSource.toString();

        if(adresseSourceReglee.startsWith("::ffff:"))
            adresseSourceReglee = adresseSourceReglee.mid(7);

        if(trame.startsWith("#"))
        {
            trame.remove(0, 1);

            float consommation = trame.toFloat();

            qDebug() << "Source : " + adresseSourceReglee +
                          " consommation : " + QString::number(consommation);
        }
    }
}
