#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QtWidgets>

/**
 * @def COLONNES_MAX
 * @brief Le nombre de colonnes max pour l'affichage des segments
 */
#define COLONNES_MAX 3

class BoiteSegment;
class CommunicationBaseDeDonnees;

class PageAccueil : public QWidget
{
    Q_OBJECT
  public:
    explicit PageAccueil(QWidget* parent = nullptr);
    virtual ~PageAccueil();
    QPushButton* getBoutonGererScenarios() const;

  private:
    QPushButton*                boutonGererScenarios;
    QGridLayout*                layoutSegments;
    QList<BoiteSegment*>        listeSegments;
    QComboBox*                  menuDeroulantScenarios;
    CommunicationBaseDeDonnees* baseDeDonnees;

    void creerSegments(const int nombreScenarios);
    void placerSegments();
    void chargerScenariosDepuisBDD();
    void chargerSegmentsDepuisBDD();

  signals:
};

#endif // PAGEACCUEIL_H
