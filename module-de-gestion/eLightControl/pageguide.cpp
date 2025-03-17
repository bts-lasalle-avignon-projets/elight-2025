#include "pageguide.h"

PageGuide::PageGuide(QWidget* parent) : QWidget(parent)
{
    QLabel* titrePageGuide = new QLabel(this);
    boutonRetourGuide      = new QPushButton(this);

    titrePageGuide->setText("<h1>Guide</h1>");
    boutonRetourGuide->setText("RETOUR");

    QVBoxLayout* layoutVertical   = new QVBoxLayout(this);
    QHBoxLayout* layoutEntetePage = new QHBoxLayout();

    layoutVertical->addLayout(layoutEntetePage);
    layoutVertical->addStretch();

    layoutEntetePage->addWidget(titrePageGuide);
    layoutEntetePage->addWidget(boutonRetourGuide);
}

QPushButton* PageGuide::getBoutonRetourGuide()
{
    return boutonRetourGuide;
}
