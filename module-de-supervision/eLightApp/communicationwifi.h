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

class CommunicationWiFi : public QObject
{
    Q_OBJECT

  public:
    CommunicationWiFi(QObject* parent = nullptr);
    ~CommunicationWiFi();

    void envoyerDatagramme(const QString& type,
                           const QString& donnees,
                           const QString& adresse,
                           quint16        port);
    int  recupererIdSegment(const QString& adresseIPSegment);
    void traiterPuissanceTrame(const QString& adresseIPSegment,
                               const QString& donnees);

  private slots:
    void lireDatagramme();

  private:
    QUdpSocket* udpSocket;
    quint16     portSocket;
    quint32     rafraichissement;

    void chargerConfiguration();
    void initialiserSocket();
    void traiterDatagramme(const QString&      datagramme,
                           const QHostAddress& adresse,
                           quint16             port);

  signals:
    void puissanceInstantaneeSegmentRecue(int idSegment, int puissance);
    void acquittementRecue(int idSegment);
    void configurationRecue(QString adresse,
                            QString nomSalle,
                            QString numeroSegment,
                            QString nbSegments,
                            QString superficieSalle);
};

#endif // COMMUNICATIONWIFI_H
