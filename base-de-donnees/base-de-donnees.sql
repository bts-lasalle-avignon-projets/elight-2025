CREATE DATABASE IF NOT EXISTS eLight;

USE eLight;

CREATE USER 'user_eLight'@'127.0.0.1' IDENTIFIED BY 'lasalle84';
GRANT SELECT, INSERT, UPDATE, DELETE ON eLight.* TO 'user_eLight'@'127.0.0.1';
FLUSH PRIVILEGES;

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
