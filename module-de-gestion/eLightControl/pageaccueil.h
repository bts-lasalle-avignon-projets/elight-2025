#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QtWidgets>
#include "communicationbasededonnees.h"
#include "communicationsegments.h"

/**
 * @def COLONNES_MAX
 * @brief Le nombre de colonnes max pour l'affichage des segments
 */
#define COLONNES_MAX 3

/**
 * @def PUISSANCE_DEFAUT
 * @brief Puissance par défaut
 */
#define PUISSANCE_DEFAUT 0

/**
 * @def ICONE_VALIDER_ACCUEIL
 * @brief L'icone sur le bouton de validation sur la page accueil
 */
#define ICONE_VALIDER_ACCUEIL "valider.png"

/**
 * @def ICONE_RETIRER_ACCUEIL
 * @brief L'icone sur le bouton de retrait sur la page accueil
 */
#define ICONE_RETIRER_ACCUEIL "retirer.png"

/**
 * @def LARGEUR_ICONES_BOUTONS_ACCUEIL
 * @brief La largeur des icones des boutons sur la page accueil en pixels
 */
#define LARGEUR_ICONES_BOUTONS_ACCUEIL 50

/**
 * @def HAUTEUR_ICONES_BOUTONS_ACCUEIL
 * @brief La hauteur des icones des boutons sur la page accueil en pixels
 */
#define HAUTEUR_ICONES_BOUTONS_ACCUEIL 50

/**
 * @def PADDING_ICONES_ACCUEIL
 * @brief L'écart entre les extrémités du bouton et les icones en pixels
 */
#define PADDING_ICONES_ACCUEIL 15

class BoiteSegment;

class PageAccueil : public QWidget
{
    Q_OBJECT
  public:
    explicit PageAccueil(QWidget* parent = nullptr);
    virtual ~PageAccueil();
    QPushButton* getBoutonGererScenarios() const;
    void         chargerScenariosDepuisBDD();
    void         chargerSegmentsDepuisBDD();
    bool         recupererNomSalle(QString& nomSalle);
    void         chargerScenarioActifDepuisBDD();
    void         selectionnerScenarioActif();
    void         retirerScenarioActif();

  private:
    QGridLayout*                layoutSegments;
    QList<BoiteSegment*>        listeSegments;
    QComboBox*                  menuDeroulantScenarios;
    CommunicationBaseDeDonnees& baseDeDonnees;
    QLabel*                     nomScenarioActif;
    QLabel*                     intensiteScenarioActif;
    int                         intensiteScenarioActifEntier;
    QPushButton*                boutonRetirerScenarioActif;
    QPushButton*                boutonConfirmerSelectionScenario;
    QVector<int>                idsSegmentsSalle;

    CommunicationSegments* communicationSegments;

    void creerSegments(const int nombreScenarios);
    void placerSegments();

  signals:
    void signalEnvoyerTrameIntensite(int idSegment, int intensite);
};

#endif // PAGEACCUEIL_H
