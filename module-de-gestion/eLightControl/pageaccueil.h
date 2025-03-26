#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QtWidgets>
#include "boitesegment.h"

/**
 * @def COLONNES_MAX
 * @brief Le nombre de colonnes max pour l'affichage des segments
 */
#define COLONNES_MAX 3

class PageAccueil : public QWidget
{
    Q_OBJECT
  public:
    explicit PageAccueil(QWidget* parent = nullptr);
    QPushButton* getBoutonGererScenarios() const;

  private:
    QPushButton*         boutonGererScenarios;
    QGridLayout*         layoutSegments;
    QList<BoiteSegment*> listeSegments;
    void                 creerSegments(const int nombreScenarios);
    void                 placerSegments();

  signals:
};

#endif // PAGEACCUEIL_H
