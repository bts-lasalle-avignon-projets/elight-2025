#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QtWidgets>
#include "config.h"
#include "communicationbasededonnees.h"

#define DEMO_HISTORIQUE

class CommunicationBaseDeDonnees;
class Historique : public QWidget
{
    Q_OBJECT
  public:
    explicit Historique(QWidget* parent = nullptr);
    virtual ~Historique();

  protected:
    void showEvent(QShowEvent* event);

  private:
    QLabel*                     titreHistorique;
    CommunicationBaseDeDonnees* baseDeDonnees;
    QListWidget*                listeHistorique;

    void initialiserTable();
    void chargerHistoriqueDepuisBDD();

  private slots:
    void fermerFenetre();

  signals:
};

#endif // HISTORIQUE_H
