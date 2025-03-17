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
 * @def PAGE_ACCUEIL
 * @brief Index de la page d'accueil
 */
#define PAGE_ACCUEIL 0

/**
 * @def PAGE_GESTION
 * @brief Index de la page de gestion des scenarios
 */
#define PAGE_GESTION 1

/**
 * @def PAGE_GUIDE
 * @brief Index de la page de guide
 */
#define PAGE_GUIDE 2

/**
 * @def PAGE_PARAMETRES
 * @brief Index de la page des paramètres
 */
#define PAGE_PARAMETRES 3

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
    void            chargerFeuilleStyle();
    void            clicBoutonChangerPage(const int index);
};

#endif // ELIGHTCONTROL_H
