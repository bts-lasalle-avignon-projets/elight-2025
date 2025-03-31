#ifndef EDITIONSALLE_H
#define EDITIONSALLE_H

#include <QtWidgets>
#include "config.h"

class EditionSalle : public QWidget
{
    Q_OBJECT
  public:
    explicit EditionSalle(QWidget* parent = nullptr);

  private slots:
    void fermerFenetre();

  signals:
};

#endif // EDITIONSALLE_H
