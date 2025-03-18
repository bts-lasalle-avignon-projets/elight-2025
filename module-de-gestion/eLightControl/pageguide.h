#ifndef PAGEGUIDE_H
#define PAGEGUIDE_H

#include <QtWidgets>

class PageGuide : public QWidget
{
    Q_OBJECT
  public:
    explicit PageGuide(QWidget* parent = nullptr);
    QPushButton* getBoutonRetourGuide() const;

  private:
    QPushButton* boutonRetourGuide;

  signals:
};

#endif // PAGEGUIDE_H
