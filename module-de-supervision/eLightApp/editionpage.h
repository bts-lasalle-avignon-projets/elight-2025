#ifndef EDITIONPAGE_H
#define EDITIONPAGE_H

#include <QWidget>

class editionPage : public QWidget {
  Q_OBJECT
public:
  explicit editionPage(QWidget *parent = nullptr);

  void retourEdit();

signals:
};

#endif // EDITIONPAGE_H
