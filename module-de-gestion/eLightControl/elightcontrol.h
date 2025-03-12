/**
 * @file elightcontrol.h
 *
 * @brief Déclaration de la classe ELightControl
 * @author Soria Bonet Enzo
 * @version 1.0
 */

#ifndef ELIGHTCONTROL_H
#define ELIGHTCONTROL_H

#include <QtWidgets>

/**
 * @def APPLICATION
 * @brief Le nom de l'application
 */
#define APPLICATION "eLightControl"

/**
 * @def VERSION
 * @brief La version de l'application
 */
#define VERSION "1.0"

/**
 * @def GAUCHE
 * @brief La position des boutons dans un layout en grille
 */
#define GAUCHE 0

/**
 * @def DROITE
 * @brief La position des boutons dans un layout en grille
 */
#define DROITE 1

/**
 * @def HAUT
 * @brief La position des boutons dans un layout en grille
 */
#define HAUT 0

/**
 * @def BAS
 * @brief La position des boutons dans un layout en grille
 */
#define BAS 1

/**
 * @class ELightControl
 * @brief Déclaration de la classe ELightControl
 * @details Cette classe gère l'interface graphique de l'application
 * ELightControl
 */
class ELightControl : public QWidget
{
    Q_OBJECT

  public:
    ELightControl(QWidget* parent = nullptr);
    ~ELightControl();

  private:
};

#endif // ELIGHTCONTROL_H
