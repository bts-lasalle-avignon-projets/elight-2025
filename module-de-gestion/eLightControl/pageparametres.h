#ifndef PAGEPARAMETRES_H
#define PAGEPARAMETRES_H

#include <QtWidgets>
#include "communicationbasededonnees.h"

class CommunicationBaseDeDonnees;

class PageParametres : public QWidget
{
    Q_OBJECT
  public:
    explicit PageParametres(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourParametre() const;
    void         validerParametreSalle();
    void         validerParametreIp();

  private:
    QPushButton*                boutonRetourParametres;
    QLineEdit*                  boiteNomDeSalle;
    QLineEdit*                  boiteIp;
    CommunicationBaseDeDonnees& baseDeDonnees;

  signals:
};

#endif // PAGEPARAMETRES_H
