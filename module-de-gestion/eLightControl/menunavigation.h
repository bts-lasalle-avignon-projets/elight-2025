#ifndef MENUNAVIGATION_H
#define MENUNAVIGATION_H

#include <QtWidgets>

/**
 * @def LOGO_APPLICATION
 * @brief Le logo de l'application eLight
 */
#define LOGO_APPLICATION "logoeLight.png"

/**
 * @def LARGEUR_LOGO
 * @brief La largeur du logo en pixels
 */
#define LARGEUR_LOGO 200

/**
 * @def HAUTEUR_LOGO
 * @brief La hauteur du logo en pixels
 */
#define HAUTEUR_LOGO 200

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

#define APPLICATION "eLightControl"

class MenuNavigation : public QWidget
{
    Q_OBJECT
  public:
    explicit MenuNavigation(QWidget* parent = nullptr);
    QPushButton* getBoutonAccueil();
    QPushButton* getBoutonGestionScenarios();
    QPushButton* getBoutonGuide();
    QPushButton* getBoutonParametres();

  private:
    QPushButton* boutonAccueil;
    QPushButton* boutonGestionScenarios;
    QPushButton* boutonGuide;
    QPushButton* boutonParametres;

  signals:
};

#endif // MENUNAVIGATION_H
