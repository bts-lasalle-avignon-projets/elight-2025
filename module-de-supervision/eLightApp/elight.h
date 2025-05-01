#ifndef ELIGHT_H
#define ELIGHT_H

#include <QtWidgets>
#include <QVector>
#include <QMap>
#include "config.h"
#include "communicationbasededonnees.h"

#define NOM_SALLE 1

#define CONSOMMATION_TOTALE 0

class Historique;
class Salle;

class ELight : public QWidget
{
    Q_OBJECT

  public:
    ELight(QWidget* parent = nullptr);
    virtual ~ELight();

  private slots:
    void afficherHistorique();
    void afficherSalle();

  private:
    Historique*                 historiquePage;
    QMap<QPushButton*, Salle*>  salles;
    QVector<QPushButton*>       boutonsSalles;
    CommunicationBaseDeDonnees* baseDeDonnees;
    QLabel*                     consommationTotaleLabel;

    void chargerSallesDepuisBDD();
    bool estSalleActive(int idSalle);
    void chargerConsommationTotaleDepuisBDD();
};

#endif // ELIGHT_H
