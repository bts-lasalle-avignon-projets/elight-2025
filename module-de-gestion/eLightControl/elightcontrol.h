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
#define CHEMIN_RESSOURCES "./ressources/"

/**
 * @def STYLE_APPLICATION
 * @brief La feuille de style de l'application eLight
 */
#define STYLE_APPLICATION "stylesheet.css"

class MenuNavigation;
class PageAccueil;
class PageGestionScenario;
class PageGuide;
class PageParametres;

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

    /**
     * @enum Page
     * @brief Définit les index des différentes pages
     *
     */
    enum Page
    {
        ACCUEIL = 0,
        GESTION,
        GUIDE,
        PARAMETRES,
        NB_PAGES
    };

  private:
    QStackedWidget* pageEmpilees;   //!< la pile de pages de la fenêtre
    MenuNavigation* menuNavigation; //!< composition du menu de navigation
    PageAccueil*    pageAccueil;    //!< composition de la page Accueil
    PageGestionScenario*
                    pageGestionScenarios; //!< composition de la page Gestion des scénarios
    PageGuide*      pageGuide;      //!< composition de la page Guide
    PageParametres* pageParametres; //!< composition de la page Parametres

    void creerPages();
    void gererNavigation();
    void personnaliserFenetre();
    void chargerFeuilleStyle();
    void changerPage(Page page);
};

#endif // ELIGHTCONTROL_H
