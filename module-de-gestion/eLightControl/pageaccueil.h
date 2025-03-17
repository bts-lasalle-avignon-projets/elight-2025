#ifndef PAGEACCUEIL_H
#define PAGEACCUEIL_H

#include <QtWidgets>

class PageAccueil : public QWidget
{
    Q_OBJECT
  public:
    explicit PageAccueil(QWidget* parent = nullptr);
    QPushButton* getBoutonGererScenarios();

  private:
    QPushButton* boutonGererScenarios;

  signals:
};

#endif // PAGEACCUEIL_H
