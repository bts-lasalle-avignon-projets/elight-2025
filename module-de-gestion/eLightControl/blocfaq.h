#ifndef BLOCFAQ_H
#define BLOCFAQ_H

#include <QWidget>
#include <QPushButton>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

class BlocFAQ : public QWidget
{
    Q_OBJECT
  public:
    BlocFAQ(const QString& question,
            const QString& reponse,
            QWidget*       parent = nullptr);
    void ouvrir();
    void fermer();
    void basculer();

  private:
    QPushButton* boutonQuestion;
    QFrame*      cadreReponse;
    QLabel*      labelReponse;

  signals:
    void demandeOuverture(BlocFAQ* source);
};

#endif // BLOCFAQ_H
