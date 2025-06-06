/**
 * @file lib/communicationudp/communicationudp.h
 * @brief Déclaration des fonctions de communication en UDP
 * @author Thierry Vaira
 * @version 0.1
 */
#ifndef COMMUNICATIONUDP_H
#define COMMUNICATIONUDP_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define CHAMP_TYPE_UDP               0
#define CHAMP_DATAS_UDP              1
#define CHAMP_DATAS_NOM_SALLE        1
#define CHAMP_DATAS_NUMERO_SEGMENT   2
#define CHAMP_DATAS_NB_SEGMENTS      3
#define CHAMP_DATAS_SUPERFICIE_SALLE 4
#define TAILLE_BUFFER                512 //!< nombre d'octets pour le buffer

/**
 * @struct ClientUDP
 *
 * @brief Un client UDP
 *
 */
struct ClientUDP
{
    IPAddress adresseIP; //!< l'adresse IP du client
    uint16_t  port;      //!< le port UDP du client
    String    message;   //!< le tampon de réception

    bool operator==(const ClientUDP& unClient) const
    {
        if(adresseIP == unClient.adresseIP && port == unClient.port)
            return true;
        return false;
    }
    bool operator!=(const ClientUDP& unClient) const
    {
        return !(*this == unClient);
    }
};

void initialiserCommunicationUDP(const IPAddress& adresseIP,
                                 uint16_t         port,
                                 bool             multicast = false);
void envoyerMessageUDP(const IPAddress& adresseIP,
                       uint16_t         port,
                       String           message);
bool recevoirMessageUDP(ClientUDP& clientUDP, char delimiteur = -1);

#endif
