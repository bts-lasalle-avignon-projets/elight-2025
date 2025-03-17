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
 * @def CHEMIN_RESSOURCES
 * @brief Le chemin vers les fichiers ressources de l'application eLight
 */
#define CHEMIN_RESSOURCES "ressources"

/**
 * @def STYLE_APPLICATION
 * @brief La feuille de style de l'application eLight
 */
#define STYLE_APPLICATION "stylesheet.css"

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
    QStackedWidget* pageEmpilees;

    void chargerFeuilleStyle();
};

#endif // ELIGHTCONTROL_H
