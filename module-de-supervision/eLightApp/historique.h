#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QWidget>

class Historique : public QWidget {
  Q_OBJECT
public:
  explicit Historique(QWidget *parent = nullptr);

  void retourHistorique();

signals:
};

#endif // HISTORIQUE_H
