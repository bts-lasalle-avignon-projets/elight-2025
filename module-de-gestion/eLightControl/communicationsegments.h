#ifndef COMMUNICATIONSEGMENTS_H
#define COMMUNICATIONSEGMENTS_H

#include <QObject>
#include <QUdpSocket>
#include <QSettings>
#include "communicationbasededonnees.h"

class CommunicationSegments : public QObject
{
    Q_OBJECT

  public:
    explicit CommunicationSegments(QObject* parent = nullptr);
    ~CommunicationSegments();
    void recupererIdSegment(QString adresseSourceReglee, int& idSegment);
    void traiterTramePuissance(const QString& ipSource, const QString& donnees);

  private:
    QUdpSocket*                 udpSocket;
    int                         portWebSocket;
    CommunicationBaseDeDonnees& baseDeDonnees;
    void                        initialiserSocket();

  signals:
    void puissanceInstantaneeSegmentRecue(int idSegment, float puissance);

  private slots:
    void traiterTrameRecue();
};

#endif // COMMUNICATIONSEGMENTS_H
