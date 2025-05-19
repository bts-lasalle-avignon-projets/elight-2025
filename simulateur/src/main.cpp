/**
 * @file src/main.cpp
 * @brief Programme principal ELIGHT 2025
 * @author Thierry Vaira
 * @version 1.0
 */
#include <Arduino.h>
#include <Preferences.h>
#include <afficheur.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <BlueDot_BME280_TSL2591.h>

// Configuration

// cf. platformio.ini (option -D)
#define NOM_SALLE "B20"
//#define NUMERO_SEGMENT 1
/*
    Ce segment est équipé de 3 Panneaux LED 120x30 29W Haut Rendement 137 lm/W
*/
#define SUPERFICIE_SALLE   150 //!< surface de la salle (en m²)
#define NB_SEGMENTS        5   //!<
#define SUPERFICIE_SEGMENT (SUPERFICIE_SALLE / NB_SEGMENTS)
#define NB_PANNEAUX        3  //!< nombre de panneaux
#define PUISSANCE_PANNEAU  29 //!< puissance d'un panneau (en W)
#define PUISSANCE_TOTALE                                                       \
    (NB_PANNEAUX * PUISSANCE_PANNEAU) //!< puissance totale (en W)
#define LUMEN_PAR_WATT 137            //!< lumen par watt
#define LUMEN_PAR_PANNEAU                                                      \
    (PUISSANCE_PANNEAU * LUMEN_PAR_WATT) //!< lumen par panneau
#define LUMINOSITE_PAR_DEFAUT 500        //!< luminosité par défaut (en lx)

#define PERIODE_SIGNALEMENT_SALLE 30000 //!< période de signalement de la salle
#define PERIODE_ACQUISITION       500   // en ms

// Choix communication
#define COMMUNICATION_UDP       1
#define COMMUNICATION_WEBSOCKET 2
#define CHOIX_COMMUNICATION     COMMUNICATION_UDP
//#define CHOIX_COMMUNICATION COMMUNICATION_WEBSOCKET

#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
#include <communicationudp.h>

#define PORT_UDP          5000         //!< numéro de port UDP
#define ADRESSE_MULTICAST "239.0.0.42" //!< l'adresse multicast
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
#include <websocket.h>

#define PORT_WEBSOCKET 5000 //!< numéro de port WebSocket
#else
#error "Choix de communication manquant ou non valide !"
#endif

// Divers
#define LONGUEUR_CHAMP 16 //!< longueur d'un champ (eeprom)

// Brochages
#define GPIO_LED_ROUGE      5  //!<
#define GPIO_LED_ORANGE     17 //!<
#define GPIO_LED_VERTE      16 //!<
#define GPIO_SW1            12 //!<
#define GPIO_SW2            14 //!<
#define GPIO_ENCODEUR_A     2
#define GPIO_ENCODEUR_B     4
#define GPIO_ENCODEUR_E     13
#define ADRESSE_I2C_OLED    0x3c //!< Adresse I2C de l'OLED
#define BROCHE_I2C_SDA      21   //!< Broche SDA
#define BROCHE_I2C_SCL      22   //!< Broche SCL
#define ADRESSE_I2C_TSL2591 0x29 //!< Adresse I2C du TSL2591

// Protocole (cf. Google Drive)
#define EN_TETE_TRAME    "#"
#define DELIMITEUR_CHAMP ";"
#define DELIMITEURS_FIN  "\r\n"
#define DELIMITEUR_DATAS ';'
#define DELIMITEUR_FIN   '\n'

// Variables globales
WiFiManager wm;          //!< gestion de la connexion WiFi
Preferences preferences; //!< pour le stockage interne
IPAddress   adresseIP;   //!< l'adresse IP local
IPAddress   broadcastIP; //!< l'adresse IP broadcast
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
WiFiUDP   udp;             //!< le serveur UDP
uint16_t  port = PORT_UDP; //!< le port de communication
IPAddress multicastIP;     //!< l'adresse IP multicast
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
WebSocketsServer webSocket =
  WebSocketsServer(PORT_WEBSOCKET);    //!< websocket serveur
uint16_t        port = PORT_WEBSOCKET; //!< le port de communication
ClientWebSocket clientWebSocket;       //!< le client WebSocket
#endif

BlueDot_BME280_TSL2591 tsl2591; //!< capteur d'éclairement lumineux
bool                   tsl2591Initialise = false;
uint32_t               luminosite        = 0;
uint32_t               intensiteScenario =
  LUMINOSITE_PAR_DEFAUT;           // en lx (NF EN 12464-1 pour les salles)
uint32_t puissanceInstantanee = 0; // en W
/*
40 W -> 440 lm
75 W -> 990 lm
100 W -> 1420 lm
150 W -> 2290 lm
200 W -> 3200 lm
*/

String strNomSalle = String(NOM_SALLE);           //!< le nom de la salle
char   prefNomSalle[LONGUEUR_CHAMP + 1];          //!< le nom de la salle
int    numeroSegment    = NUMERO_SEGMENT;         //!< le numéro de segment
String strNumeroSegment = String(NUMERO_SEGMENT); //!< le numéro de segment
char   prefNumeroSegment[LONGUEUR_CHAMP + 1];     //!< la surface de la salle
String nomESP32 =
  "elight-" + strNomSalle + "-" + strNumeroSegment; //!< le nom de l'ESP32
unsigned long tempsPrecedentSignalement = 0;
unsigned long tempsPrecedentAcquisition = 0;
bool          refresh = false; //!< demande rafraichissement de l'écran OLED
bool          resetEnCours = false; //!< demande de reset
bool          antiRebond   = false; //!< anti-rebond
Afficheur     afficheur(ADRESSE_I2C_OLED,
                    BROCHE_I2C_SDA,
                    BROCHE_I2C_SCL);             //!< afficheur OLED SSD1306
String        entete     = String(EN_TETE_TRAME);    // caractère séparateur
String        separateur = String(DELIMITEUR_CHAMP); // caractère séparateur
String        delimiteurFin = String(DELIMITEURS_FIN); // fin de message

void configurerTSL2591()
{
    tsl2591.parameter.I2CAddress = ADRESSE_I2C_TSL2591;

    // 0b00:    Low gain mode
    // 0b01:    Medium gain mode
    // 0b10:    High gain mode
    // 0b11:    Maximum gain mode

    tsl2591.parameter.gain = 0b01;

    // 0b000:   100ms (max count = 37888)
    // 0b001:   200ms (max count = 65535)
    // 0b010:   300ms (max count = 65535)
    // 0b011:   400ms (max count = 65535)
    // 0b100:   500ms (max count = 65535)
    // 0b101:   600ms (max count = 65535)

    tsl2591.parameter.integration = 0b000;

    tsl2591.config_TSL2591();
#ifdef DEBUG
    Serial.println("[elight] TSL2591     : " +
                   String(ADRESSE_I2C_TSL2591, HEX));
    Serial.println("[elight] Gain        : medium");
    Serial.println("[elight] Integration : 100 ms");
#endif

    if(tsl2591.init_TSL2591() != 0x50)
    {
        tsl2591Initialise = false;
#ifdef DEBUG
        Serial.println("[elight] TSL2591     : " + String("non détecté !"));
#endif

        /*while(true)
            ;*/
    }
    else
    {
        tsl2591Initialise = true;
    }
}

bool acquerirEclairement()
{
    if(!tsl2591Initialise)
        return false;

    luminosite = uint32_t(tsl2591.readIlluminance_TSL2591());
#ifdef DEBUG
    // Serial.println("[elight] Luminosite : " + String(luminosite) +
    // String(" lx"));
#endif

    return true;
}

uint32_t calculerPuissanceInstantanee()
{
    if(luminosite < intensiteScenario)
    {
        puissanceInstantanee =
          uint32_t(float(LUMEN_PAR_PANNEAU) / float(SUPERFICIE_SEGMENT));
    }
    else
    {
        puissanceInstantanee = 0;
    }
#ifdef DEBUG
    // Serial.println("[elight] Puissance : " + String(puissanceInstantanee) +
    //               String(" W"));
#endif
    return puissanceInstantanee;
}

String extraireChampMessage(const String& message, unsigned int numeroChamp)
{
    String messageTemp = message;
    messageTemp.replace(entete, "");
    String       champ;
    unsigned int compteurCaractere  = 0;
    unsigned int compteurDelimiteur = 0;
    char         fin                = '\n';

    if(delimiteurFin.length() > 0)
        fin = delimiteurFin[0];

    for(unsigned int i = 0; i < messageTemp.length(); i++)
    {
        if(messageTemp[i] == separateur[0] || messageTemp[i] == fin)
        {
            compteurDelimiteur++;
        }
        else if(compteurDelimiteur == numeroChamp)
        {
            champ += messageTemp[i];
            compteurCaractere++;
        }
    }

    return champ;
}

bool verifierMessage(const String& message)
{
    if(message.length() <= delimiteurFin.length())
    {
#ifdef DEBUG
        Serial.println("[elight] erreur longueur !");
#endif
        return false;
    }
    if(!message.startsWith(entete))
    {
#ifdef DEBUG
        Serial.println("[elight] erreur entete !");
#endif
        return false;
    }
    if(!message.endsWith(delimiteurFin))
    {
#ifdef DEBUG
        Serial.println("[elight] erreur delimiteur de fin !");
#endif
        return false;
    }
    return true;
}

/**
 * @brief Retourne true si l'échéance de la durée fixée a été atteinte
 * @param intervalle unsigned long
 * @return bool true si la durée est arrivée à échéance
 */
bool estEcheanceSignalement(unsigned long intervalle)
{
    if(intervalle == 0)
        return false;
    unsigned long temps = millis();
    if(temps - tempsPrecedentSignalement >= intervalle)
    {
        tempsPrecedentSignalement = temps;
        return true;
    }
    return false;
}

bool estEcheanceAcquisition(unsigned long intervalle)
{
    if(intervalle == 0)
        return false;
    unsigned long temps = millis();
    if(temps - tempsPrecedentAcquisition >= intervalle)
    {
        tempsPrecedentAcquisition = temps;
        return true;
    }
    return false;
}

void chargerInformations()
{
#ifdef DEBUG
    Serial.println("[elight] charge les informations stockées en eeprom");
#endif
    // initialise le stockage interne
    if(preferences.getBytes("salle", prefNomSalle, LONGUEUR_CHAMP) > 0)
    {
        strNomSalle = String(prefNomSalle);
    }
    else
        strNomSalle = String(NOM_SALLE);
    if(preferences.getBytes("segment", prefNumeroSegment, LONGUEUR_CHAMP) > 0)
    {
        strNumeroSegment = String(prefNumeroSegment);
    }
    else
        strNumeroSegment = String(NUMERO_SEGMENT);
}

void reinitialiserAffichage()
{
    afficheur.setMessageLigne(Afficheur::Ligne1, ""); //
    afficheur.setMessageLigne(Afficheur::Ligne2, ""); //
    afficheur.setMessageLigne(Afficheur::Ligne3, ""); //
    afficheur.setMessageLigne(Afficheur::Ligne4, ""); //
    refresh = true;
}

void afficherWifi(String message, bool etat)
{
    afficheur.setMessageLigne(Afficheur::Ligne1, message);
    if(etat)
        afficheur.setMessageLigne(Afficheur::Ligne2, "SSID " + WiFi.SSID());
    else
        afficheur.setMessageLigne(Afficheur::Ligne2,
                                  "Point acces " + wm.getDefaultAPName());
    afficheur.afficher();
}

void afficherReset()
{
    afficheur.setMessageLigne(Afficheur::Ligne1, "Reset parametres WiFi");
    afficheur.setMessageLigne(Afficheur::Ligne2, "SSID " + WiFi.SSID());
    afficheur.setMessageLigne(Afficheur::Ligne3, "Redemarrage");
    afficheur.setMessageLigne(Afficheur::Ligne4, "dans 2 secondes ...");
    afficheur.afficher();
    delay(2000);
}

void afficherAccueil()
{
    String strChoixCommunication;
    String strNumeroPort;
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
    strChoixCommunication = "UDP";
    strNumeroPort         = String(PORT_UDP);
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
    strChoixCommunication = "WS";
    strNumeroPort         = String(PORT_WEBSOCKET);
#else
    strChoixCommunication = "";
    strNumeroPort         = "";
#endif
    afficheur.setMessageLigne(Afficheur::Ligne1,
                              strChoixCommunication + " " +
                                WiFi.localIP().toString() + ":" +
                                strNumeroPort);
    afficheur.setMessageLigne(Afficheur::Ligne2,
                              "Salle " + strNomSalle + " - " + "Segment " +
                                strNumeroSegment);
    afficheur.setMessageLigne(Afficheur::Ligne3, "");
    afficheur.setMessageLigne(Afficheur::Ligne4, "");
    afficheur.afficher();
}

/**
 * @brief Déclenchée par interruption sur le bouton SW1
 * @fn demanderReset()
 */
void IRAM_ATTR demanderReset()
{
    if(antiRebond || resetEnCours)
        return;

    resetEnCours = true;
    antiRebond   = true;
}

void lancerReset()
{
#ifdef DEBUG
    Serial.println("[elight] reset !");
#endif
    afficherReset();
    wm.resetSettings();
    ESP.restart();
}

/**
 * @brief Initialise les ressources du programme
 *
 * @fn setup
 *
 */
void setup()
{
    Serial.begin(115200);
    while(!Serial)
        ;
    delay(250);
    Wire.begin();

#ifdef DEBUG
    Serial.println("[elight] elight 2025");
    Serial.println("[elight] salle   : " + String(NOM_SALLE));
    Serial.println("[elight] segment : " + String(NUMERO_SEGMENT));
    Serial.println("[elight] adresse MAC : " + WiFi.macAddress());
#endif

    // Configuration E/S
    pinMode(GPIO_LED_ROUGE, OUTPUT);
    pinMode(GPIO_LED_ORANGE, OUTPUT);
    pinMode(GPIO_LED_VERTE, OUTPUT);
    pinMode(GPIO_SW1, INPUT_PULLUP);
    digitalWrite(GPIO_LED_ROUGE, HIGH);
    digitalWrite(GPIO_LED_ORANGE, LOW);
    digitalWrite(GPIO_LED_VERTE, LOW);

    // Pour le reset
    attachInterrupt(digitalPinToInterrupt(GPIO_SW1), demanderReset, FALLING);

    // Initialise le capteur TSL2591
    configurerTSL2591();

    // Initialise l'afficheur OLED
    afficheur.initialiser();
    String titre     = "            elight 2025";
    String soustitre = String("== ") + WiFi.macAddress() + String(" ==");

    afficheur.setTitre(titre);
    afficheur.setSTitre(soustitre);
    afficherWifi("Demarrage Wifi", false);

    // Initialise le stockage interne
    preferences.begin("eeprom", false); // false pour r/w

    // Gestion WiFi
    wm.setTitle("BTS CIEL LaSalle Avignon");
    // wm.setDarkMode(true);

    bool connecte = wm.autoConnect();
    if(!connecte)
    {
        digitalWrite(GPIO_LED_ROUGE, HIGH);
        digitalWrite(GPIO_LED_ORANGE, LOW);
        digitalWrite(GPIO_LED_VERTE, LOW);
#ifdef DEBUG
        Serial.println("[elight] WiFiManager : connexion echec !");
#endif
        // ESP.restart();
    }
    else
    {
        digitalWrite(GPIO_LED_ROUGE, LOW);
        digitalWrite(GPIO_LED_ORANGE, LOW);
        digitalWrite(GPIO_LED_VERTE, HIGH);
#ifdef DEBUG
        Serial.println("[elight] WiFiManager : connexion ok !");
#endif
        // delay(1000);
        MDNS.begin(nomESP32.c_str());
    }

    // wm.setConfigResetCallback(lancerReset);

    afficherAccueil();
    // chargerInformations();

    // initialise le générateur pseudo-aléatoire
    esp_random();

    adresseIP   = WiFi.localIP();
    broadcastIP = WiFi.broadcastIP();

#ifdef DEBUG
    Serial.println("[elight] adresse MAC : " + WiFi.macAddress());
    Serial.println("[elight] Adresse IP : " + adresseIP.toString());
    Serial.println("[elight] Adresse broadcast : " + broadcastIP.toString());
#endif
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
    multicastIP.fromString(ADRESSE_MULTICAST);
#ifdef DEBUG
    Serial.println("[elight] Adresse multicast : " + multicastIP.toString());
#endif
    initialiserCommunicationUDP(adresseIP, port);
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
    initialiserWebSocket();
#endif

    tempsPrecedentSignalement = millis();
    tempsPrecedentAcquisition = millis();
}

/**
 * @brief Boucle infinie d'exécution du programme
 *
 * @fn loop
 *
 */
void loop()
{
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
    ClientUDP clientUDP;
#endif

    if(refresh)
    {
        afficheur.afficher();
        refresh = false;
    }

    if(antiRebond)
    {
        afficheur.afficher();
        delay(250);
        antiRebond = false;
    }

    if(resetEnCours)
    {
        resetEnCours = false;
#ifdef DEBUG
        Serial.println("[elight] demande reset !");
#endif
        lancerReset();
    }

    if(estEcheanceSignalement(PERIODE_SIGNALEMENT_SALLE)) // timeout
    {
        String messageSignalement = String(EN_TETE_TRAME) + strNomSalle +
                                    separateur + strNumeroSegment +
                                    delimiteurFin;
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
        envoyerMessageUDP(broadcastIP, port, messageSignalement);
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
        envoyerMessageWebSocket(messageSignalement);
#endif
    }

    if(estEcheanceAcquisition(PERIODE_ACQUISITION)) // acquisition
    {
        if(acquerirEclairement())
        {
            puissanceInstantanee = calculerPuissanceInstantanee();
            afficheur.setMessageLigne(Afficheur::Ligne4,
                                      String(luminosite) + " lx -> " +
                                        String(puissanceInstantanee) + " W");
            afficheur.afficher();
        }
    }

#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
    if(recevoirMessageUDP(clientUDP, DELIMITEUR_FIN))
    {
        if(verifierMessage(clientUDP.message))
        {
            String typeMessage =
              extraireChampMessage(clientUDP.message, CHAMP_TYPE_UDP);
            String datasMessage =
              extraireChampMessage(clientUDP.message, CHAMP_DATAS_UDP);
#ifdef DEBUG
            Serial.println("[elight] type message  : " + typeMessage);
            Serial.println("[elight] datas message : " + datasMessage);
#endif
            switch(typeMessage[0])
            {
                case 'I': // Intensité
                    intensiteScenario =
                      datasMessage.toInt(); // conversion en entier
                    afficheur.setMessageLigne(Afficheur::Ligne3,
                                              "I : " + datasMessage + " lx");
                    afficheur.afficher();
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
                    envoyerMessageUDP(broadcastIP,
                                      port,
                                      String(EN_TETE_TRAME) + String("A") +
                                        separateur + String("0") +
                                        delimiteurFin);
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
                    envoyerMessageWebSocket(String(EN_TETE_TRAME) +
                                            String("A") + separateur +
                                            String("0") + delimiteurFin);
#endif
                    break;
                case 'P': // Demande puissance
                    puissanceInstantanee = calculerPuissanceInstantanee();
#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
                    envoyerMessageUDP(
                      broadcastIP,
                      port,
                      String(EN_TETE_TRAME) + String("P") + separateur +
                        String(puissanceInstantanee) + delimiteurFin);
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
                    envoyerMessageWebSocket(
                      String(EN_TETE_TRAME) + String("P") + separateur +
                      String(puissanceInstantanee) + delimiteurFin);
#endif
                    break;
                case 'A': // Acquittement

                    break;
                default:
#ifdef DEBUG
                    Serial.println("[elight] type message inconnu !");
#endif
            }
        }
    }
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
    webSocket.loop();
#endif
}