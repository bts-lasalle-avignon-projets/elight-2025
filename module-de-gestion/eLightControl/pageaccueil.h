#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QtWidgets>
#include "boitesegment.h"

class PageAccueil : public QWidget
{
    Q_OBJECT
  public:
    explicit PageAccueil(QWidget* parent = nullptr);
    QPushButton* getBoutonGererScenarios() const;

  private:
    QPushButton*         boutonGererScenarios;
    QHBoxLayout*         layoutSegments;
    QList<BoiteSegment*> listeSegments;
    void                 creerSegments(const int nombreScenarios);
    void                 placerSegments();

  signals:
};

#endif // PAGEACCUEIL_H
