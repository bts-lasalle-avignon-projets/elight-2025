/**
 * @file lib/communicationudp/websocket.h
 * @brief Déclaration des fonctions de communication websocket
 * @author Thierry Vaira
 * @version 0.1
 */
#ifndef WEBSOCKET_H
#define WEBSOCKET_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsServer.h>

#define CHAMP_SALLE_WEBSOCKET 0
#define CHAMP_DATAS_WEBSOCKET 1

#define INTERVALLE_PING_WEBSOCKET 10000 //!< 10 s
#define PONG_TIMEOUT_WEBSOCKET    5000  //!< délai d'attente pour le pong
#define NB_TIMEOUT_WEBSOCKET      2 //!< nombre de timeout avant déconnexion

/**
 * @struct ClientWebSocket
 *
 * @brief Un client WebSocket
 *
 */
struct ClientWebSocket
{
    uint8_t numero = -1; //!< le numéro du client
    String  message;     //!< le tampon de réception

    bool operator==(const ClientWebSocket& unClient) const
    {
        if(numero == unClient.numero)
            return true;
        return false;
    }
    bool operator!=(const ClientWebSocket& unClient) const
    {
        return !(*this == unClient);
    }
};

void   traiterEvenementWebSocket(uint8_t  numero,
                                 WStype_t type,
                                 uint8_t* payload,
                                 size_t   length);
void   initialiserWebSocket();
void   envoyerMessageWebSocket(uint8_t numero, String message);
void   envoyerMessageWebSocket(String message);
String extraireChampMessageWebSocket(const String& message,
                                     unsigned int  numeroChamp);
bool   verifierMessageWebSocket(const String& message);

#endif