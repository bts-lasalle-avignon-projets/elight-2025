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

// Configuration

// cf. platformio.ini (option -D)
#define NOM_SALLE "B20"
//#define NUMERO_SEGMENT 1
#define PERIODE_SIGNALEMENT_SALLE 10000 //!< période de signalement de la salle

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
#define GPIO_LED_ROUGE   5  //!<
#define GPIO_LED_ORANGE  17 //!<
#define GPIO_LED_VERTE   16 //!<
#define GPIO_SW1         12 //!<
#define GPIO_SW2         14 //!<
#define GPIO_ENCODEUR_A  2
#define GPIO_ENCODEUR_B  4
#define GPIO_ENCODEUR_E  13
#define ADRESSE_I2C_OLED 0x3c //!< Adresse I2C de l'OLED
#define BROCHE_I2C_SDA   21   //!< Broche SDA
#define BROCHE_I2C_SCL   22   //!< Broche SCL

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

String strNomSalle = String(NOM_SALLE);           //!< le nom de la salle
char   prefNomSalle[LONGUEUR_CHAMP + 1];          //!< le nom de la salle
int    numeroSegment    = NUMERO_SEGMENT;         //!< le numéro de segment
String strNumeroSegment = String(NUMERO_SEGMENT); //!< le numéro de segment
char   prefNumeroSegment[LONGUEUR_CHAMP + 1];     //!< la surface de la salle
String nomESP32 =
  "elight-" + strNomSalle + "-" + strNumeroSegment; //!< le nom de l'ESP32
unsigned long tempsPrecedent = 0;
bool          refresh = false; //!< demande rafraichissement de l'écran OLED
bool          resetEnCours = false; //!< demande de reset
bool          antiRebond   = false; //!< anti-rebond
Afficheur     afficheur(ADRESSE_I2C_OLED,
                    BROCHE_I2C_SDA,
                    BROCHE_I2C_SCL);             //!< afficheur OLED SSD1306
String        entete     = String(EN_TETE_TRAME);    // caractère séparateur
String        separateur = String(DELIMITEUR_CHAMP); // caractère séparateur
String        delimiteurFin = String(DELIMITEURS_FIN); // fin de message

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
    if(message.length() < delimiteurFin.length())
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
bool estEcheance(unsigned long intervalle)
{
    if(intervalle == 0)
        return false;
    unsigned long temps = millis();
    if(temps - tempsPrecedent >= intervalle)
    {
        tempsPrecedent = temps;
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
    afficheur.setMessageLigne(Afficheur::Ligne2, "Salle " + strNomSalle);
    afficheur.setMessageLigne(Afficheur::Ligne3, "Segment " + strNumeroSegment);
    afficheur.setMessageLigne(Afficheur::Ligne4, "");
    afficheur.afficher();

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
    // Serial.println(randomSeed(analogRead(34)));
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

    if(estEcheance(PERIODE_SIGNALEMENT_SALLE)) // timeout
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

#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
    if(recevoirMessageUDP(clientUDP, DELIMITEUR_FIN))
    {
        if(verifierMessage(clientUDP.message))
        {
            String nomSalleMessage =
              extraireChampMessage(clientUDP.message, CHAMP_SALLE_UDP);
            String datasMessage =
              extraireChampMessage(clientUDP.message, CHAMP_DATAS_UDP);
#ifdef DEBUG
            Serial.println("[elight] nom salle   : " + nomSalleMessage);
            Serial.println("[elight] datas salle : " + datasMessage);
#endif
            // echo
            envoyerMessageUDP(clientUDP.adresseIP,
                              clientUDP.port,
                              clientUDP.message);
        }
    }
#elif CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
    webSocket.loop();
#endif
}
