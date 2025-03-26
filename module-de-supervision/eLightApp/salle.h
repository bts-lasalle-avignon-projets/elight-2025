#ifndef SALLE_H
#define SALLE_H

#include <QtWidgets>
#include "config.h"

class Salle : public QWidget
{
    Q_OBJECT
  public:
    explicit Salle(QString nom, QWidget* parent = nullptr);
    virtual ~Salle();

  private:
    QString nom;
    QLabel* titre;
    QLabel* consommation;

  protected:
    void showEvent(QShowEvent* event);

  private slots:
    void fermerFenetre();
    void editerSalle();

  signals:
};

#endif // SALLE_H
