#ifndef PAGEGESTIONSCENARIO_H
#define PAGEGESTIONSCENARIO_H

#include <QtWidgets>
#include "communicationbasededonnees.h"

/**
 * @def INTENSITE_MIN
 * @brief L'intensité lumineuse minimale
 */
#define INTENSITE_MIN 0

/**
 * @def INTENSITE_MAX
 * @brief L'intensité lumineuse maximale
 */
#define INTENSITE_MAX 750

/**
 * @def ICONE_VALIDER_GESTION
 * @brief L'icone sur le bouton de validation sur la page de gestion
 */
#define ICONE_VALIDER_GESTION "valider.png"

/**
 * @def ICONE_SUPPRIMER_GESTION
 * @brief L'icone sur le bouton de suppression sur la page de gestion
 */
#define ICONE_SUPPRIMER_GESTION "poubelle.png"

/**
 * @def LARGEUR_ICONES_BOUTONS_GESTION
 * @brief La largeur des icones des boutons sur la page de gestion en pixels
 */
#define LARGEUR_ICONES_BOUTONS_GESTION 50

/**
 * @def HAUTEUR_ICONES_BOUTONS_GESTION
 * @brief La hauteur des icones des boutons sur la page accueil en pixels
 */
#define HAUTEUR_ICONES_BOUTONS_GESTION 50

/**
 * @def PADDING_ICONES_GESTION
 * @brief L'écart entre les extrémités du bouton et les icones en pixels
 */
#define PADDING_ICONES_GESTION 15

class PageGestionScenario : public QWidget
{
    Q_OBJECT
  public:
    explicit PageGestionScenario(QWidget* parent = nullptr);
    void chargerScenariosDepuisBDD();
    void enregistrerScenario();
    void supprimerScenario();
    void modifierScenario();

  private:
    CommunicationBaseDeDonnees& baseDeDonnees;
    QLineEdit*                  boiteNomScenario;
    QSlider*                    sliderIntensiteScenario;
    QLabel*                     labelValeurIntensiteScenario;
    QComboBox*                  listeScenarios;
    QLineEdit*                  boiteNouveauNom;
    QSlider*                    sliderNouvelleIntensite;
    QLabel*                     labelValeurNouvelleIntensite;

  signals:
};

#endif // PAGEGESTIONSCENARIO_H
