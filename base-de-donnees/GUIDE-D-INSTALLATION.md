# Guide d'installation de la base de données

## MySQL

[MySQL](https://www.mysql.com/fr/) est un système de gestion de bases de données relationnelles (SGBDR). Il fonctionne en **client/serveur**.

Il est distribué sous une double licence GPL et propriétaire. Il fait partie des logiciels de gestion de base de données les plus utilisés au monde, autant par le grand public (applications web principalement) que par des professionnels, en concurrence avec Oracle, Informix, PostgreSQL, et Microsoft SQL Server.

MySQL fonctionne sur de nombreux systèmes d’exploitation différents, incluant GNU/Linux, Mac OS X et Windows. Les bases de données sont accessibles en utilisant les langages de programmation C, C++, VisualBasic, VB .NET, C#, Delphi/Kylix, Eiffel, Java, Perl, PHP, Python, Windev, Ruby et Tcl. Une API spécifique est disponible pour chacun d’entre eux.

> En 2009, à la suite du rachat de MySQL par Sun Microsystems et des annonces du rachat de Sun Microsystems par Oracle Corporation, Michael Widenius, fondateur de MySQL, quitte cette société pour lancer le projet MariaDB, dans une démarche visant à remplacer MySQL tout en assurant l’interopérabilité. Le nom vient de la 2e fille de Michael Widenius, Maria (la première s’appelant My). MariaDB a été choisi par défaut sur les distributions « Debian ».

Le serveur peut être installé à partir des paquets ou en utilisant un conteneur [Docker](https://dev.mysql.com/doc/mysql-installation-excerpt/8.0/en/docker-mysql-getting-started.html).

### Solution n°1 : paquet Ubuntu

```sh
$ sudo apt install mysql-server

$ mysql --version

$ sudo systemctl status mysql.service

$ mysql -h 127.0.0.1 -u root -ppassword -e "show databases;"
```

### Solution n°2 : Docker

- Installation `docker-ce` :

```sh
$ sudo apt update
$ sudo apt install ca-certificates curl gnupg
$ sudo install -m 0755 -d /etc/apt/keyrings
$ curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /etc/apt/keyrings/docker.gpg
$ sudo chmod a+r /etc/apt/keyrings/docker.gpg
$ echo   "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(. /etc/os-release && echo "$VERSION_CODENAME") stable" |   sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
$ sudo apt update

$ sudo apt install docker-ce docker-ce-cli containerd.io docker-buildx-plugin docker-compose-plugin

$ docker --version
$ docker compose version
$ docker version
```

- Création et démarrage du serveur MySQL :

> Vérifier que le serveur MySQL ne s'exécute pas en local : `systemctl status mysql.service` sinon il faut l'arrêter avec `sudo systemctl stop mysql.service`.

```sh
$ docker compose up -d
```

> Sinon pour simplement démarrer le service mysql : `$ docker compose start mysql`

- Vérifications :

```sh
$ docker compose ls

$ docker compose ps
$ docker ps -a

$ docker images

$ docker compose top
```

- Redémarrer le service mysql :

```bash
$ docker compose restart mysql
```

- Arrêter le service mysql :

```bash
$ docker compose stop mysql
```

- Si besoin, supprimer l'image `mysql` :

```bash
$ docker compose rm mysql
$ docker rmi $(docker images mysql -q)
```

## Base de données eLight

```sql
CREATE DATABASE IF NOT EXISTS eLight;

USE eLight;

CREATE TABLE IF NOT EXISTS scenario (
    id_scenario INT PRIMARY KEY AUTO_INCREMENT,
    nom_scenario VARCHAR(255) NOT NULL,
    intensite_scenario INT NOT NULL DEFAULT 200,
    date_creation_scenario TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

CREATE TABLE IF NOT EXISTS salle (
    id_salle INT PRIMARY KEY AUTO_INCREMENT,
    nom_salle VARCHAR(255) UNIQUE NOT NULL,
    ip_boitier VARCHAR(15) UNIQUE NOT NULL
);

CREATE TABLE IF NOT EXISTS segment (
    id_segment INT PRIMARY KEY AUTO_INCREMENT,
    id_salle INT NOT NULL,
    id_scenario INT NULL,
    ip_segment VARCHAR(15) UNIQUE NOT NULL,
    CONSTRAINT fk_segment_salle FOREIGN KEY (id_salle) REFERENCES salle(id_salle) ON DELETE CASCADE,
    CONSTRAINT fk_segment_scenario FOREIGN KEY (id_scenario) REFERENCES scenario(id_scenario) ON DELETE SET NULL
);

CREATE TABLE IF NOT EXISTS historique_consommation_segment (
    id_historique INT PRIMARY KEY AUTO_INCREMENT,
    id_segment INT,
    consommation FLOAT NOT NULL,
    horodatage_releve TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (id_segment) REFERENCES segment(id_segment)
);
```

- Installation :

```sh
$ mysql -h 127.0.0.1 -u root -ppassword eLight < base-de-donnes.sql

$ mysql -h 127.0.0.1 -u root -ppassword eLight -e "show tables;"
```

## Qt

Dans le fichier `.pro` :

```txt
QT  += sql
```

Installation du paquet fournissant le plugin MySQL pour Qt 5 :

```sh
$ sudo apt-get install libqt5sql5-mysql
```
