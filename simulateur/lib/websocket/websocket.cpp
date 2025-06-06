/**
 * @file lib/communicationudp/websocket.cpp
 * @brief Définition des fonctions de communication websocket
 * @author Thierry Vaira
 * @version 0.1
 */
#include <websocket.h>

#if CHOIX_COMMUNICATION == COMMUNICATION_WEBSOCKET
extern WebSocketsServer webSocket;       //!< le serveur websocket
extern ClientWebSocket  clientWebSocket; //!< le client websocket
extern String           entete;          //!< caractère séparateur
extern String           separateur;      //!< caractère séparateur
extern String           delimiteurFin;   //!< fin de message
#endif

/**
 * @brief Gestion des évènements
 */
void traiterEvenementWebSocket(uint8_t  numero,
                               WStype_t type,
                               uint8_t* payload,
                               size_t   length)
{
    switch(type)
    {
        case WStype_DISCONNECTED:
#ifdef DEBUG
            Serial.printf("[WS] déconnecté (%u)\n", numero);
#endif
            clientWebSocket.numero = -1;
            clientWebSocket.message.clear();
            break;
        case WStype_CONNECTED:
        {
#ifdef DEBUG
            IPAddress ip = webSocket.remoteIP(numero);
            Serial.printf("[WS] connecté à l'adresse IP : %d.%d.%d.%d (%u)\n",
                          ip[0],
                          ip[1],
                          ip[2],
                          ip[3],
                          numero);
#endif
            clientWebSocket.numero = numero;
            clientWebSocket.message.clear();
        }
        break;
        case WStype_TEXT:
            clientWebSocket.message = String((char*)payload);
            clientWebSocket.numero  = numero;
            if(!clientWebSocket.message.endsWith(delimiteurFin))
            {
                clientWebSocket.message += delimiteurFin;
            }
#ifdef DEBUG
            String messageDebug = clientWebSocket.message;
            messageDebug.replace(delimiteurFin, "\\r\\n");
            Serial.println("[WS] reception du message \"" + messageDebug +
                           "\" (" + String(numero) + ")");
#endif
            if(verifierMessageWebSocket(clientWebSocket.message))
            {
                String nomSalleMessage =
                  extraireChampMessageWebSocket(clientWebSocket.message,
                                                CHAMP_SALLE_WEBSOCKET);
                String datasMessage =
                  extraireChampMessageWebSocket(clientWebSocket.message,
                                                CHAMP_DATAS_WEBSOCKET);
#ifdef DEBUG
                Serial.println("[WS] nom salle   : " + nomSalleMessage);
                Serial.println("[WS] datas salle : " + datasMessage);
#endif
                // echo
                envoyerMessageWebSocket(numero, clientWebSocket.message);
                clientWebSocket.message.clear();
            }

            break;
    }
}

void initialiserWebSocket()
{
#ifdef DEBUG
    Serial.println("[WS] initialisation de la websocket");
#endif
    webSocket.begin();
    webSocket.onEvent(traiterEvenementWebSocket);
    webSocket.enableHeartbeat(INTERVALLE_PING_WEBSOCKET,
                              PONG_TIMEOUT_WEBSOCKET,
                              NB_TIMEOUT_WEBSOCKET);
}

void envoyerMessageWebSocket(uint8_t numero, String message)
{
    if(message.isEmpty())
    {
        return;
    }
#ifdef DEBUG
    String messageDebug = message;
    messageDebug.replace("\r\n", "\\r\\n");
    Serial.println("[WS] envoi le message \"" + messageDebug + "\" au client " +
                   String(numero));
#endif
    webSocket.sendTXT(numero, message);
}

void envoyerMessageWebSocket(String message)
{
    if(message.isEmpty())
    {
        return;
    }
#ifdef DEBUG
    String messageDebug = message;
    messageDebug.replace("\r\n", "\\r\\n");
    Serial.println("[WS] envoi le message \"" + messageDebug +
                   "\" en broadcast ");
#endif
    webSocket.broadcastTXT(message);
}

String extraireChampMessageWebSocket(const String& message,
                                     unsigned int  numeroChamp)
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

bool verifierMessageWebSocket(const String& message)
{
    if(message.length() < delimiteurFin.length())
    {
#ifdef DEBUG
        Serial.println("[WS] erreur longueur !");
#endif
        return false;
    }
    if(!message.startsWith(entete))
    {
#ifdef DEBUG
        Serial.println("[WS] erreur entete !");
#endif
        return false;
    }
    if(!message.endsWith(delimiteurFin))
    {
#ifdef DEBUG
        Serial.println("[WS] erreur delimiteur de fin !");
#endif
        return false;
    }
    return true;
}
