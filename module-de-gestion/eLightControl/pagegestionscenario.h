#ifndef PAGEGESTIONSCENARIO_H
#define PAGEGESTIONSCENARIO_H

#include <QtWidgets>
#include "communicationbasededonnees.h"

class CommunicationBaseDeDonnees;

class PageGestionScenario : public QWidget
{
    Q_OBJECT
  public:
    explicit PageGestionScenario(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourGestionScenario() const;

  private:
    QPushButton*                boutonRetourGestionScenario;
    CommunicationBaseDeDonnees& baseDeDonnees;

  signals:
};

#endif // PAGEGESTIONSCENARIO_H
