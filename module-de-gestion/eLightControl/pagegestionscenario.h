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

class CommunicationBaseDeDonnees;

class PageGestionScenario : public QWidget
{
    Q_OBJECT
  public:
    explicit PageGestionScenario(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourGestionScenario() const;
    void         chargerScenariosDepuisBDD();
    void         enregistrerScenario();
    void         supprimerScenario();
    void         modifierScenario();

  private:
    QPushButton*                boutonRetourGestionScenario;
    CommunicationBaseDeDonnees& baseDeDonnees;
    QLineEdit*                  boiteNomScenario;
    QLineEdit*                  boiteIntensiteScenario;
    QComboBox*                  listeScenarios;
    QLineEdit*                  boiteNouveauNom;
    QLineEdit*                  boiteNouvelleIntensite;

  signals:
};

#endif // PAGEGESTIONSCENARIO_H
