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
    BlocFAQ(const QString& question, const QString& reponse, QWidget* parent = nullptr);

private:
    QPushButton* bouton;
    QFrame*      cadreReponse;
    QLabel*      labelReponse;
};

#endif // BLOCFAQ_H
