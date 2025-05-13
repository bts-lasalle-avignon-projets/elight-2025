#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QtWidgets>
#include "config.h"
#include "communicationbasededonnees.h"

#define ID_SEGMENT        0
#define CONSOMMATION      1
#define HORODATAGE_RELEVE 2

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
