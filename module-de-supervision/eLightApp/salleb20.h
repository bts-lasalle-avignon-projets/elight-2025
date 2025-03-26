#ifndef SALLEB20_H
#define SALLEB20_H

#include <QWidget>

class salleB20 : public QWidget {
  Q_OBJECT
public:
  explicit salleB20(QWidget *parent = nullptr);

  void retourB20();
  void editionB20();

signals:
};

#endif // SALLEB20_H
