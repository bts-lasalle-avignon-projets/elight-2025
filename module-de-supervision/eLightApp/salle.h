#ifndef SALLE_H
#define SALLE_H

#include <QtWidgets>
#include "config.h"
#include "salle.h"
#include "communicationbasededonnees.h"

class CommunicationBaseDeDonnees;
class EditionSalle;

class Salle : public QWidget
{
    Q_OBJECT
  public:
    explicit Salle(QString nom, QWidget* parent = nullptr);
    virtual ~Salle();

    void rechargerDonnees();

  private:
    QString                     nom;
    QLabel*                     titre;
    QLabel*                     consommation;
    EditionSalle*               editionPage;
    CommunicationBaseDeDonnees* baseDeDonnees;
    QComboBox*                  menuScenario;
    QComboBox*                  menuSegment;

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
