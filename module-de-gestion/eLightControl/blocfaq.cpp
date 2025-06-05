#include "blocfaq.h"
#include "config.h"
#include <QStyle>

BlocFAQ::BlocFAQ(const QString& question, const QString& reponse, QWidget* parent)
    : QWidget(parent)
{
    bouton = new QPushButton(question);
    cadreReponse = new QFrame;
    labelReponse = new QLabel(reponse);

    labelReponse->setWordWrap(true);
    labelReponse->setObjectName("labelReponseGuide");
    cadreReponse->setVisible(false);
    cadreReponse->setLayout(new QVBoxLayout);
    cadreReponse->layout()->addWidget(labelReponse);
    cadreReponse->setFrameShape(QFrame::NoFrame);
    cadreReponse->raise();

    bouton->setObjectName("boutonFAQferme");

    cadreReponse->setObjectName("cadreReponseFAQ");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(bouton);
    layout->addWidget(cadreReponse);

    connect(bouton, &QPushButton::clicked, this, [=]() {
                cadreReponse->setVisible(!cadreReponse->isVisible());
                if (bouton->objectName() == "boutonFAQferme") {
                    bouton->setObjectName("boutonFAQouvert");
                } else {
                    bouton->setObjectName("boutonFAQferme");
                }
                bouton->style()->unpolish(bouton);
                bouton->style()->polish(bouton);
            });
}
