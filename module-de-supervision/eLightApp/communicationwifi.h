#ifndef COMMUNICATIONWIFI_H
#define COMMUNICATIONWIFI_H

#include <QObject>
#include <QUdpSocket>
#include <QHostAddress>
#include <QMap>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

#define PORT_DEFAUT 5000

class communicationWiFi : public QObject
{
    Q_OBJECT

  public:
    communicationWiFi(QObject* parent = nullptr);
    ~communicationWiFi();

    void envoyerTrame(const QString&      type,
                      const QString&      donnees,
                      const QHostAddress& adresse,
                      quint16             port);
    void recupererIdSegment(QString adresseSourceReglee, int& idSegment);
    void traiterPuissanceTrame(const QString& ipSource, const QString& donnees);

  private slots:
    void lireTrame();

  private:
    QUdpSocket* udpSocket;
    quint16     portSocket = PORT_DEFAUT;
    void        initialiserSocket();
    void        traiterTrame(const QString&      trame,
                             const QHostAddress& sender,
                             quint16             senderPort);
};

#endif // COMMUNICATIONWIFI_H
