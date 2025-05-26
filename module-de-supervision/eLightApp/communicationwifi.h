#ifndef COMMUNICATIONWIFI_H
#define COMMUNICATIONWIFI_H

#include <QObject>
#include <QUdpSocket>

// Divers
#define PORT_DEFAUT             5000
#define RAFRAICHISSEMENT_DEFAUT 1000
#define SEGMENT_INDEFINI        -1

// Protocole
#define DELIMITEUR_DEBUT "#"
#define DELIMITEUR_FIN   "\r\n"

// Datagrammes
#define NB_CHAMPS    2
#define CHAMP_TYPE   0
#define CHAMP_DONNEE 1

// Datagramme configuration
#define NB_CHAMPS_CONFIGURATION 5
#define CHAMP_NOM_SALLE         1
#define CHAMP_NUMERO_SEGMENT    2
#define CHAMP_NB_SEGMENTS       3
#define CHAMP_SUPERFICIE_SALLE  4

#define ELEMENTS_MIN_TRAME          2
#define ELEMENTS_MAX_TRAME          5
#define TYPE_TRAME                  0
#define PUISSANCE_INSTANTANEE_TRAME 1
#define NOM_SALLE_TRAME             1
#define NUMERO_SEGMENT_TRAME        2
#define NOMBRE_SEGMENTS_TRAME       3
#define SUPERFICIE_SALLE            4

class CommunicationWiFi : public QObject
{
    Q_OBJECT

  public:
    CommunicationWiFi(QObject* parent = nullptr);
    ~CommunicationWiFi();

    void    envoyerDatagramme(const QString& type,
                              const QString& donnees,
                              const QString& adresse,
                              quint16        port);
    int     recupererIdSegment(const QString& adresseIPSegment);
    void    traiterDatagramme(const QString&      datagramme,
                              const QHostAddress& adresse,
                              quint16             port);
    bool    recupererPort(int& port);
    void    envoyerTrameDemandePuissance(const QString& adresse);
    void    envoyerTrameIntensite(const QString& adresse, const int& intensite);
    QString recupererAdresseDestination(const int& idSegment);
    QStringList recupererAdressesDestinations();
    bool        recupererNomSalle(QString& adresse);
    QString     recupererAdresseDestination();
    void        chargerConfiguration();
    void        initialiserSocket();

  private slots:
    void lireDatagramme();

  private:
    QUdpSocket* udpSocket;
    quint16     portSocket;
    quint32     rafraichissement;
    QUdpSocket* udpSocketEmission;
    int         portWebSocket;

  signals:
    void puissanceInstantaneeSegmentRecue(QString adresse,
                                          int     idSegment,
                                          int     puissance);
    void acquittementRecue(QString adresse, int idSegment);
    void configurationRecue(QString adresse,
                            QString nomSalle,
                            QString numeroSegment,
                            QString nbSegments,
                            QString superficieSalle);
};

#endif // COMMUNICATIONWIFI_H
