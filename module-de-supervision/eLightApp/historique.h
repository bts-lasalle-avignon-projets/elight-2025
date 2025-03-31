#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QtWidgets>
#include "config.h"

#define DEMO_HISTORIQUE

class Historique : public QWidget
{
    Q_OBJECT
  public:
    explicit Historique(QWidget* parent = nullptr);
    virtual ~Historique();

  protected:
    void showEvent(QShowEvent* event);

  private:
    QLabel*       titreHistorique;
    QTableWidget* table;

    void initialiserTable();

  private slots:
    void fermerFenetre();

  signals:
};

#endif // HISTORIQUE_H
