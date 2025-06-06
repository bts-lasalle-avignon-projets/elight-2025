USE eLight;

INSERT INTO salle (id_salle, nom_salle, ip_boitier) VALUES
(1, 'B20', '192.168.20.1'),
(2, 'B22', '192.168.22.1');

INSERT INTO scenario (id_scenario, nom_scenario,intensite_scenario, date_creation_scenario) VALUES
(1, 'Scenario matin', 200, CURRENT_TIMESTAMP),
(2, 'Scenario après-midi', 150, CURRENT_TIMESTAMP);

INSERT INTO segment (id_segment,id_salle,id_scenario,ip_segment) VALUES
(1, 1, 1, '192.168.20.2'),
(2, 1, 2, '192.168.20.3');
