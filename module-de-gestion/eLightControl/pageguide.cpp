#include "pageguide.h"
#include "config.h"
#include <QDebug>

PageGuide::PageGuide(QWidget* parent) : QWidget(parent)
{
    QLabel* titrePageGuide = new QLabel(this);

    QFont police;
    police.setPointSize(TAILLE_POLICE);

    titrePageGuide->setFont(police);

    titrePageGuide->setText("<h1>Guide</h1><br>");

    QVBoxLayout* layoutVertical   = new QVBoxLayout(this);
    QHBoxLayout* layoutEntetePage = new QHBoxLayout();

    layoutVertical->addLayout(layoutEntetePage);
    layoutVertical->addStretch();

    layoutEntetePage->addWidget(titrePageGuide);
}
