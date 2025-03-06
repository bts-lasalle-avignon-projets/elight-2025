# Projet eLight 💡

- [Projet eLight 💡](#projet-elight-)
  - [Présentation](#présentation)
    - [Module de gestion par salle](#module-de-gestion-par-salle)
    - [Module de supervision](#module-de-supervision)
  - [Itérations](#itérations)
    - [Itération 1](#itération-1)
    - [Itérations 2](#itérations-2)
    - [Itérations 3](#itérations-3)
    - [Itérations 4](#itérations-4)
  - [Recette](#recette)
    - [Recette - Module de Gestion par salle](#recette---module-de-gestion-par-salle)
    - [Recette - Module de Supervision](#recette---module-de-supervision)
  - [Diagrammes](#diagrammes)
  - [Base de données](#base-de-données)
  - [Utilisation](#utilisation)
  - [Changelog](#changelog)
  - [Documentation](#documentation)
  - [Créateurs](#créateurs)

---

## Présentation

Le projet eLight vise à optimiser l'éclairage des structures telles que les supermarchés, entrepôts et salles de classe en ajustant la lumière en fonction des besoins réels des occupants. Grâce à des capteurs de luminosité et un contrôle intelligent, il permet de réaliser des économies d'énergie tout en garantissant un confort visuel optimal.

### Module de gestion par salle

Ce module permet au client de gérer l’éclairage d’une salle. Il peut ainsi choisir un scénario d’éclairage, visualiser la consommation en cours, créer un nouveau scénario pour sa salle et gérer les scénarios existants. Il envoie des instructions aux modules contrôleur de segment.

### Module de supervision

Ce module permet au client de superviser toutes les salles équipées de système elight. Il peut visualiser les états de tous les segments, gérer les scénarios de toutes les salles, gérer les attributions de segment à une salle, suivre la consommation de toute l’installation à l’instant T et visualiser l’historique des consommations pour toute l’installation.

## Itérations

### Itération 1

- **Créer des scénarios** : L'utilisateur peut créer un scénario.
- **Supprimer des scénarios** : L'utilisateur peut supprimer un scénario.
- **Mettre à jour des scénarios** : L'utilisateur peut mettre à jour un scénario.
- **Afficher les scénarios enregistrés** : L'utilisateur peut visualiser les scénarios disponibles.

### Itérations 2

- **Sélectionner un scénario** : L'utilisateur peut sélectionner pour la salle.
- **Afficher le scénario actif** : L'utilisateur peut visualiser le scénario actif de la salle.
- **Afficher la consommation des segments** : L'utilisateur peut visualiser la consommation des segments dans la salle.

### Itérations 3

- **Afficher la consommation instantanée** : L'utilisateur peut voir la consommation à un instant T via l'application de supervision.
- **Afficher une page de guide** : L'utilisateur peut accéder à une page d'aide.

### Itérations 4

- **Interface graphique revisitée** : Amélioration de l'interface graphique.

## Recette

### Recette - Module de Gestion par salle

| Objectif | À faire | En cours | Achevé |
|----------|---------|----------|--------|
| Consultation des segments de la salle | ☐ | ☐ | ☐ |
| Création des scénarios d’éclairage | ☐ | ☐ | ☐ |
| Suppression des scénarios d’éclairage | ☐ | ☐ | ☐ |
| Mise à jour des scénarios d’éclairage | ☐ | ☐ | ☐ |
| Sélectionner un scénario pour la salle | ☐ | ☐ | ☐ |
| Synchronisation avec la base de données centrale | ☐ | ☐ | ☐ |
| Réception de la trame via le WIFI | ☐ | ☐ | ☐ |
| Envoi de la trame via le WIFI | ☐ | ☐ | ☐ |

### Recette - Module de Supervision

| Objectif | À faire | En cours | Achevé |
|----------|---------|----------|--------|
| Paramétrage des salles | ☐ | ☐ | ☐ |
| Consultation de l'état des salles | ☐ | ☐ | ☐ |
| Création des scénarios d’éclairage | ☐ | ☐ | ☐ |
| Suppression des scénarios d’éclairage | ☐ | ☐ | ☐ |
| Mise à jour des scénarios d’éclairage | ☐ | ☐ | ☐ |
| Suivi et historique de la consommation | ☐ | ☐ | ☐ |
| Synchronisation avec la base de données centrale | ☐ | ☐ | ☐ |
| Réception de la trame via le WIFI | ☐ | ☐ | ☐ |
| Envoi de la trame via le WIFI | ☐ | ☐ | ☐ |

## Diagrammes

> TODO

## Base de données

> TODO

## Utilisation

> TODO

## Changelog

> TODO

## Documentation

> TODO

## Créateurs

- **[Soria Bonet Enzo](https://github.com/esoriabonet)** : *Module de gestion par salle*
- **[Blondel Joshua](https://github.com/JBLONDEL04)** : *Module de supervision*
