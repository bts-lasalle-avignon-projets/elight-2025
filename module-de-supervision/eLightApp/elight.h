/**
 * @file eLight.h
 *
 * @brief Déclaration de la classe ELight
 * @author Blondel Joshua
 * @version 1.0
 */

#ifndef ELIGHT_H
#define ELIGHT_H

#include <QtWidgets>
#include <QVector>
#include <QMap>
#include "config.h"

#define DEMO_ELIGHT 3 // Le nombre de salles à simuler

class Historique;
class Salle;

/**
 * @class ELight
 * @brief Déclaration de la classe ELight
 * @details Cette classe gère l'interface graphique de l'application
 * ELightControl
 */
class ELight : public QWidget
{
    Q_OBJECT

  public:
    ELight(QWidget* parent = nullptr);
    ~ELight();

  private slots:
    void afficherHistorique();
    void afficherSalle();

  private:
    Historique*                historiquePage;
    QMap<QPushButton*, Salle*> salles;
    QVector<QPushButton*>      boutonsSalles;

    void initialiserSalles();
};

#endif // ELIGHT_H
