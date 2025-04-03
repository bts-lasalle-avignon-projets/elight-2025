#ifndef PAGEPARAMETRES_H
#define PAGEPARAMETRES_H

#include <QtWidgets>

class PageParametres : public QWidget
{
    Q_OBJECT
  public:
    explicit PageParametres(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourParametre() const;
    void         validerParametreSalle();
    void         validerParametreIp();

  private:
    QPushButton* boutonRetourParametres;
    QLineEdit*   boiteNomDeSalle;
    QLineEdit*   boiteIp;

  signals:
};

#endif // PAGEPARAMETRES_H
