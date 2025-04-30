#ifndef COMMUNICATIONSEGMENTS_H
#define COMMUNICATIONSEGMENTS_H

#include <QObject>
#include <QUdpSocket>
#include <QSettings>

class CommunicationSegments : public QObject
{
    Q_OBJECT

  public:
    explicit CommunicationSegments(QObject* parent = nullptr);
    ~CommunicationSegments();

  private:
    QUdpSocket* udpSocket;
    int         portWebSocket;
    void        initialiserSocket();

  signals:

  private slots:
    void traiterTrameRecue();
};

#endif // COMMUNICATIONSEGMENTS_H
