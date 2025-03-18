#ifndef PAGEGESTIONSCENARIO_H
#define PAGEGESTIONSCENARIO_H

#include <QtWidgets>

class PageGestionScenario : public QWidget
{
    Q_OBJECT
  public:
    explicit PageGestionScenario(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourGestionScenario() const;

  private:
    QPushButton* boutonRetourGestionScenario;
  signals:
};

#endif // PAGEGESTIONSCENARIO_H
