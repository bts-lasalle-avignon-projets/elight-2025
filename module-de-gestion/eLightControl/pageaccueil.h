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
    QPushButton*                boutonGererScenarios;
    QGridLayout*                layoutSegments;
    QList<BoiteSegment*>        listeSegments;
    QComboBox*                  menuDeroulantScenarios;
    CommunicationBaseDeDonnees& baseDeDonnees;
    QLabel*                     nomScenarioActif;
    QLabel*                     intensiteScenarioActif;
    QVector<int>                idsSegmentsSalle;

    CommunicationSegments* communicationSegments;

    void creerSegments(const int nombreScenarios);
    void placerSegments();

  signals:
};

#endif // PAGEACCUEIL_H
