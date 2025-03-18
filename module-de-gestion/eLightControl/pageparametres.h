#ifndef PAGEPARAMETRES_H
#define PAGEPARAMETRES_H

#include <QtWidgets>

class PageParametres : public QWidget
{
    Q_OBJECT
  public:
    explicit PageParametres(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourParametre() const;

  private:
    QPushButton* boutonRetourParametres;

  signals:
};

#endif // PAGEPARAMETRES_H
