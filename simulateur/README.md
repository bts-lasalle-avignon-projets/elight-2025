# Simulateur ELIGHT 2025

## Présentation du protocole implanté dans le simulateur ESP'ACE

Ce document présente rapidement le fonctionnement du simulateur ainsi que le protocole implémenté.

> Le protocole complet est disponible dans Google Drive.

## Configuration du simulateur

```cpp
// cf. platformio.ini (option -D)
#define NOM_SALLE "B20"
//#define NUMERO_SEGMENT 1
#define PERIODE_SIGNALEMENT_SALLE 10000 //!< période de signalement de la salle

// Choix communication
#define COMMUNICATION_UDP       1
#define COMMUNICATION_WEBSOCKET 2
#define CHOIX_COMMUNICATION     COMMUNICATION_UDP
//#define CHOIX_COMMUNICATION COMMUNICATION_WEBSOCKET
```

> Le bouton `SW1` permet d'effectuer un _reset_ de la configuration WiFi.

## Test UDP

- Démarrage du simulateur (moniteur série) :

```txt
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
...
[elight] elight 2025
[elight] salle   : B20
[elight] segment : 1
[elight] adresse MAC : 80:7D:3A:C4:2A:B8
*wm:AutoConnect
*wm:Connecting to SAVED AP: Livebox-XXXX
*wm:connectTimeout not set, ESP waitForConnectResult...
*wm:AutoConnect: SUCCESS
*wm:STA IP Address: 10.0.0.36
[elight] WiFiManager : connexion ok !
[elight] adresse MAC : 80:7D:3A:C4:2A:B8
[elight] Adresse IP : 10.0.0.36
[elight] Adresse broadcast : 10.0.0.255
[elight] Adresse multicast : 239.0.0.42
[UDP] initialisation sur le port 5000
[UDP] envoi le message "#B20;1\r\n" à 10.0.0.255:5000
[UDP] envoi le message "#B20;1\r\n" à 10.0.0.255:5000
[UDP] envoi le message "#B20;1\r\n" à 10.0.0.255:5000
[UDP] reception du message "#B20;1\r\n" de 10.0.0.22:54371
[elight] nom salle   : B20
[elight] datas salle : 1
[UDP] envoi le message "#B20;1\r\n" à 10.0.0.22:54371
...
```

- Sur le PC :

```sh
$ nc -C -u 10.0.0.36 5000
#B20;1
#B20;1
```

> Ici, le simulateur est en mode **echo**.

Ou en _broadcast_ :

```sh
$ nc.traditional -C -b -l -u -p 5000
...
```

## Test WebSocket

- Démarrage du simulateur (moniteur série) :

```txt
rst:0x1 (POWERON_RESET),boot:0x13 (SPI_FAST_FLASH_BOOT)
...
[elight] elight 2025
[elight] salle   : B20
[elight] segment : 1
[elight] adresse MAC : 80:7D:3A:C4:2A:B8
*wm:AutoConnect
*wm:Connecting to SAVED AP: Livebox-XXXX
*wm:connectTimeout not set, ESP waitForConnectResult...
*wm:AutoConnect: SUCCESS
*wm:STA IP Address: 10.0.0.36
[elight] WiFiManager : connexion ok !
[elight] adresse MAC : 80:7D:3A:C4:2A:B8
[elight] Adresse IP : 10.0.0.36
[elight] Adresse broadcast : 10.0.0.255
[WS] initialisation de la websocket
[WS] envoi le message "#B20;1\r\n" en broadcast
[WS] Connecté à l'adresse IP : 10.0.0.22 (0)
[WS] envoi le message "#B20;1\r\n" en broadcast
[WS] reception du message "#B20;100\r\n" (0)
[WS] nom salle   : B20
[WS] datas salle : 100
[WS] envoi le message "#B20;100\r\n" au client 0
[WS] envoi le message "#B20;1\r\n" en broadcast
[WS] envoi le message "#B20;1\r\n" en broadcast
[WS] déconnecté (0)
...
```

- Sur le PC :

> cf. [https://github.com/bts-lasalle-avignon-ressources/WebSocket/#cli](https://github.com/bts-lasalle-avignon-ressources/WebSocket/#cli)

```sh
$ rlwrap websocat ws://10.0.0.36:5000
#B20;1
#B20;100
#B20;100
#B20;1
#B20;1
^C
```

Ou :

```sh
$ wscat -c ws://10.0.0.36:5000
Connected (press CTRL+C to quit)
< #B20;1

> #B20;100
< #B20;100

< #B20;1

< #B20;1
```

> Ici, le simulateur est en mode **echo**.

## Protocole

- Version : 0.1
- Trame ASCII
- Délimiteur de début : **#**
- Délimiteur de fin : **\\r\\n**
- Séparateur de champs : **;**

## platformio.ini

```ini
; PlatformIO Project Configuration File

[env]
platform = espressif32
;board = esp32dev
board = lolin32
framework = arduino
;platform_packages = platformio/framework-arduinoespressif32@^3.20006.0
lib_deps =
  thingpulse/ESP8266 and ESP32 OLED driver for SSD1306 displays @ ^4.2.0
  bblanchon/ArduinoJson @ ^6.21.2
  https://github.com/tzapu/WiFiManager.git
  links2004/WebSockets @ ^2.4.1
upload_speed = 115200
monitor_speed = 115200

[env:esp32_segment_1]
build_flags = -D$PIOENV -DNUMERO_SEGMENT=1 -DDEBUG
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0

[env:esp32_segment_2]
build_flags = -D$PIOENV -DNUMERO_SEGMENT=1 -DDEBUG
upload_port = /dev/ttyUSB1
monitor_port = /dev/ttyUSB1
```

## Auteur

- Thierry Vaira <<tvaira@free.fr>>
