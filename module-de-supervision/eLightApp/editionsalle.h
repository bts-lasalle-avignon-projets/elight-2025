#ifndef EDITIONSALLE_H
#define EDITIONSALLE_H

#include <QtWidgets>
#include "config.h"

class Salle;
class CommunicationBaseDeDonnees;

class EditionSalle : public QWidget
{
    Q_OBJECT
  public:
    explicit EditionSalle(Salle* salle, QWidget* parent = nullptr);
    virtual ~EditionSalle();

  private:
    Salle*                      salle;
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

  private slots:
    void fermerFenetre();
    void sauvegarderFenetreScenarios();
    void sauvegarderFenetreSegments();

  signals:
    void scenarioModifie();
};

#endif // EDITIONSALLE_H
