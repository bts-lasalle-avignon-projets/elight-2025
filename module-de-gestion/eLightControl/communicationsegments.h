#ifndef COMMUNICATIONSEGMENTS_H
#define COMMUNICATIONSEGMENTS_H

#include <QObject>
#include <QUdpSocket>
#include <QSettings>
#include <QTimer>
#include <QtWidgets>
#include "communicationbasededonnees.h"

/**
 * @def ELEMENTS_MIN_TRAME
 * @brief Le nombre minimal d'élément dans la trame
 */
#define ELEMENTS_MIN_TRAME 2

/**
 * @def ELEMENTS_MAX_TRAME
 * @brief Le nombre maximal d'élément dans la trame
 */
#define ELEMENTS_MAX_TRAME 5

/**
 * @def TYPE_TRAME
 * @brief Position du type dans la trame
 */
#define TYPE_TRAME 0

/**
 * @def PUISSANCE_INSTANTANEE_TRAME
 * @brief Position de la puissance instantanée dans la trame
 */
#define PUISSANCE_INSTANTANEE_TRAME 1

/**
 * @def NOM_SALLE_TRAME
 * @brief Position de la puissance instantanée dans la trame
 */
#define NOM_SALLE_TRAME 1

/**
 * @def NUMERO_SEGMENT_TRAME
 * @brief Position du nom de la salle dans la trame
 */
#define NUMERO_SEGMENT_TRAME 2

/**
 * @def NOMBRE_SEGMENTS_TRAME
 * @brief Position du nombre de segments dans la trame
 */
#define NOMBRE_SEGMENTS_TRAME 3

/**
 * @def SUPERFICIE_SALLE
 * @brief Position de la superficie de la salle dans la trame
 */
#define SUPERFICIE_SALLE 4

class CommunicationSegments : public QObject
{
    Q_OBJECT

  public:
    explicit CommunicationSegments(QObject* parent = nullptr);
    ~CommunicationSegments();
    void recupererIdSegment(QString adresseSourceReglee, int& idSegment);
    void traiterTramePuissance(const QString& ipSource, const QString& donnees);
    bool initialiserTempsRafraichissement();
    int  getTempsRafraichissement();
    bool recupererPort(int& port);
    void envoyerTrameUDP(const QString& adresse);
    QString     recupererAdresseDestination(const int& idSegment);
    QStringList recupererAdressesDestinations();
    bool        recupererNomSalle(QString& adresse);

  private:
    QUdpSocket*                 udpSocket;
    int                         portWebSocket;
    CommunicationBaseDeDonnees& baseDeDonnees;
    QUdpSocket*                 udpSocketEmission;
    QTimer*                     timerEmission;
    int                         tempsRafraichissement;
    void                        initialiserSocket();

  signals:
    void puissanceInstantaneeSegmentRecue(int idSegment, float puissance);

  private slots:
    void traiterTrameRecue();
};

#endif // COMMUNICATIONSEGMENTS_H
