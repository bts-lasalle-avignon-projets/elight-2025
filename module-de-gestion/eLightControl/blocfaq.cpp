#include "blocfaq.h"
#include "config.h"
#include <QStyle>
#include <QDebug>

BlocFAQ::BlocFAQ(const QString& question,
                 const QString& reponse,
                 QWidget*       parent) :
    QWidget(parent)
{
    boutonQuestion = new QPushButton(question);
    cadreReponse   = new QFrame;
    labelReponse   = new QLabel(reponse);

    labelReponse->setWordWrap(true);
    labelReponse->setObjectName("labelReponseGuide");
    cadreReponse->setVisible(false);
    cadreReponse->setLayout(new QVBoxLayout);
    cadreReponse->layout()->addWidget(labelReponse);
    cadreReponse->setFrameShape(QFrame::NoFrame);
    cadreReponse->raise();

    boutonQuestion->setObjectName("boutonFAQferme");

    cadreReponse->setObjectName("cadreReponseFAQ");

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    layout->addWidget(boutonQuestion);
    layout->addWidget(cadreReponse);

    connect(boutonQuestion, &QPushButton::clicked, this, [=]() {
        emit demandeOuverture(this);
    });
}

void BlocFAQ::ouvrir()
{
    cadreReponse->setVisible(true);
    boutonQuestion->setObjectName("boutonFAQouvert");
    boutonQuestion->style()->unpolish(boutonQuestion);
    boutonQuestion->style()->polish(boutonQuestion);
}

void BlocFAQ::fermer()
{
    cadreReponse->setVisible(false);
    boutonQuestion->setObjectName("boutonFAQferme");
    boutonQuestion->style()->unpolish(boutonQuestion);
    boutonQuestion->style()->polish(boutonQuestion);
}

void BlocFAQ::basculer()
{
    if(cadreReponse->isVisible())
        fermer();
    else
        ouvrir();
}
