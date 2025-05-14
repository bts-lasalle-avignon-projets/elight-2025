#include "communicationsegments.h"
#include <QDebug>

CommunicationSegments::CommunicationSegments(QObject* parent) :
    QObject(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
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
            trame.remove(0, 1);

            QStringList elements = trame.split(";");

            if(elements.size() != 2)
            {
                qWarning() << Q_FUNC_INFO << " Trame invalide " << trame;
                return;
            }

            QString type    = elements[0].trimmed();
            QString donnees = elements[1].trimmed();

            QString adresseSourceReglee = adresseSource.toString();

            if(adresseSourceReglee.startsWith("::ffff:"))
                adresseSourceReglee = adresseSourceReglee.mid(7);

            if(type == "C")
            {
                traiterTramePuissance(adresseSourceReglee, donnees);
            }
        }
    }
}

void CommunicationSegments::traiterTramePuissance(const QString& ipSource,
                                                  const QString& donnees)
{
    int idSegment;

    recupererIdSegment(ipSource, idSegment);

    float consommation = donnees.toFloat();

    emit consommationSegmentRecue(idSegment, consommation);

    qDebug() << "Source : " + ipSource + " idSegment : " + idSegment +
                  " consommation : " + QString::number(consommation);
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
