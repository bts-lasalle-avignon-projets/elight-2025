#ifndef MENUNAVIGATION_H
#define MENUNAVIGATION_H

#include <QtWidgets>

/**
 * @def LOGO_APPLICATION
 * @brief Le logo de l'application eLight
 */
#define LOGO_APPLICATION "logoeLight.png"

/**
 * @def LARGEUR_LOGO_APPLICATION
 * @brief La largeur du logo en pixels
 */
#define LARGEUR_LOGO_APPLICATION 230

/**
 * @def HAUTEUR_LOGO_APPLICATION
 * @brief La hauteur du logo en pixels
 */
#define HAUTEUR_LOGO_APPLICATION 230

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
 * @def ICONE_ACCUEIL
 * @brief L'icone sur le bouton de la page d'accueil du menu de navigation
 */
#define ICONE_ACCUEIL "accueil.png"

/**
 * @def ICONE_GESTION
 * @brief L'icone sur le bouton de la page de gestion du menu de navigation
 */
#define ICONE_GESTION "gestion.png"

/**
 * @def ICONE_GUIDE
 * @brief L'icone sur le bouton de la page de guide du menu de navigation
 */
#define ICONE_GUIDE "info.png"

/**
 * @def ICONE_PARAMETRE
 * @brief L'icone sur le bouton de la page des paramètres du menu de navigation
 */
#define ICONE_PARAMETRE "parametre.png"

/**
 * @def LARGEUR_ICONES_BOUTONS
 * @brief La largeur des icones des boutons en pixels
 */
#define LARGEUR_ICONES_BOUTONS 80

/**
 * @def HAUTEUR_ICONES_BOUTONS
 * @brief La hauteur des icones des boutons en pixels
 */
#define HAUTEUR_ICONES_BOUTONS 80

/**
 * @def PADDING_ICONES
 * @brief L'écart entre les extrémités du bouton et les icones en pixels
 */
#define PADDING_ICONES 40

#define APPLICATION "eLightControl"

class MenuNavigation : public QWidget
{
    Q_OBJECT
  public:
    explicit MenuNavigation(QWidget* parent = nullptr);
    QPushButton* getBoutonAccueil() const;
    QPushButton* getBoutonGestionScenarios() const;
    QPushButton* getBoutonGuide() const;
    QPushButton* getBoutonParametres() const;

  private:
    QPushButton* boutonAccueil;
    QPushButton* boutonGestionScenarios;
    QPushButton* boutonGuide;
    QPushButton* boutonParametres;

  signals:
};

#endif // MENUNAVIGATION_H
