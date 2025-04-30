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
        QByteArray trame;
        trame.resize(static_cast<int>(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(trame.data(), trame.size());

        qDebug() << Q_FUNC_INFO << "Trame reçue : " << trame;
    }
}
