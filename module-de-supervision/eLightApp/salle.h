#ifndef SALLE_H
#define SALLE_H

#include <QtWidgets>
#include "config.h"
#include "salle.h"
#include "communicationbasededonnees.h"

#define COLONNE_ID_SALLE 0

#define COLONNE_ID_SCENARIO        0
#define COLONNE_NOM_SCENARIO       1
#define COLONNE_INTENSITE_SCENARIO 2

#define COLONNE_ID_SEGMENT        0
#define COLONNE_IP_SEGMENT        1
#define CHAMP_CONSOMMATION_EXISTE 2

#define COLONNE_CONSOMMATION_BDD 0

class CommunicationBaseDeDonnees;
class EditionSalle;

class Salle : public QWidget
{
    Q_OBJECT
  public:
    explicit Salle(QString nom, QWidget* parent = nullptr);
    virtual ~Salle();

    void rechargerDonnees();
    int  getIdSalle() const
    {
        return idSalle;
    }

  private:
    QString                     nom;
    int                         idSalle;
    QLabel*                     titre;
    QLabel*                     consommation;
    EditionSalle*               editionPage;
    CommunicationBaseDeDonnees* baseDeDonnees;
    QListWidget*                menuScenario;
    QListWidget*                menuSegment;

    void chargerSegmentsDepuisBDD();
    void chargerScenariosDepuisBDD();
    void chargerConsommationDepuisBDD();

  protected:
    void showEvent(QShowEvent* event);

  private slots:
    void fermerFenetre();
    void editerSalle();

  signals:
};

#endif // SALLE_H
