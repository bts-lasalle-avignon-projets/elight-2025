#ifndef EDITIONSALLE_H
#define EDITIONSALLE_H

#include <QtWidgets>
#include "config.h"
#include "communicationbasededonnees.h"
#include "salle.h"

class CommunicationBaseDeDonnees;
class EditionSalle : public QWidget
{
    Q_OBJECT
  public:
    explicit EditionSalle(Salle* salle, QWidget* parent = nullptr);

  private:
    CommunicationBaseDeDonnees* baseDeDonnees;
    QComboBox*                  menuScenarios;
    QComboBox*                  menuSegments;
    QLineEdit*                  ajoutIPSegment;
    QLineEdit*                  ajoutScenario;
    QLineEdit*                  ajoutIntensiteScenario;

    void chargerScenariosDepuisBDD();
    void chargerSegmentsDepuisBDD();

    void supprimerSegmentsBDD();
    void modifierSegmentsBDD();
    void ajouterSegmentsBDD();

    void supprimerScenariosBDD();
    void modifierScenariosBDD();
    void ajouterScenariosBDD();

    Salle* salle;

  private slots:
    void fermerFenetre();
    void sauvegarderFenetreScenarios();
    void sauvegarderFenetreSegments();

  signals:
};

#endif // EDITIONSALLE_H
