/**
 * @file lib/communicationudp/communicationudp.cpp
 * @brief Définition des fonctions de communication en UDP
 * @author Thierry Vaira
 * @version 0.1
 */
#include <communicationudp.h>

#if CHOIX_COMMUNICATION == COMMUNICATION_UDP
extern WiFiUDP udp;           //!< le serveur UDP
extern String  delimiteurFin; //!< fin de message
#endif

void initialiserCommunicationUDP(const IPAddress& adresseIP,
                                 uint16_t         port,
                                 bool             multicast)
{
    if(multicast)
    {
#ifdef DEBUG
        Serial.println("[UDP] initialisation multicast sur le port " +
                       String(port));
#endif
        udp.beginMulticast(adresseIP, port);
    }
    else
    {
#ifdef DEBUG
        Serial.println("[UDP] initialisation sur le port " + String(port));
#endif
        if(adresseIP.toString().isEmpty())
            udp.begin(port);
        else
            udp.begin(adresseIP, port);
    }
}

void envoyerMessageUDP(const IPAddress& adresseIP,
                       uint16_t         port,
                       String           message)
{
    if(adresseIP.toString().isEmpty() || port == 0 || message.isEmpty())
    {
        return;
    }
#ifdef DEBUG
    String messageDebug = message;
    messageDebug.replace("\r\n", "\\r\\n");
    Serial.println("[UDP] envoi le message \"" + messageDebug + "\" à " +
                   adresseIP.toString() + ":" + String(port));
#endif

    udp.beginPacket(adresseIP, port);
    udp.write((uint8_t*)message.c_str(), message.length());
    udp.endPacket();
}

bool recevoirMessageUDP(ClientUDP& clientUDP, char delimiteur)
{
    if(udp.parsePacket() > 0)
    {
        // pas de délimiteur ?
        if(delimiteur == -1)
        {
            char     buffer[TAILLE_BUFFER + 1];
            uint16_t longueurDatagramme = udp.available();
#ifdef DEBUG
            Serial.println("[UDP] longueur datagramme recu = " +
                           String(longueurDatagramme));
#endif
            udp.read(buffer, longueurDatagramme);
            buffer[longueurDatagramme] = '\0';
            clientUDP.message          = String(buffer);
            clientUDP.message.trim();
            if(!clientUDP.message.endsWith(delimiteurFin))
            {
                clientUDP.message += delimiteurFin;
            }
        }
        else
        {
            clientUDP.message.clear();
            int c = udp.read();
            while(c >= 0 && c != delimiteur)
            {
                clientUDP.message += (char)c;
                c = udp.read();
            }
            if(c == delimiteur)
            {
                clientUDP.message += delimiteur;
            }
            else
            {
                clientUDP.message += delimiteurFin;
            }
        }
        clientUDP.adresseIP = udp.remoteIP();
        clientUDP.port      = udp.remotePort();
#ifdef DEBUG
        if(clientUDP.message.length() > delimiteurFin.length())
        {
            String messageDebug = clientUDP.message;
            messageDebug.replace("\r\n", "\\r\\n");
            Serial.println("[UDP] reception du message \"" + messageDebug +
                           "\" de " + udp.remoteIP().toString() + ":" +
                           String(udp.remotePort()));
        }
#endif
        return true;
    }
    else
    {
        return false;
    }
}
