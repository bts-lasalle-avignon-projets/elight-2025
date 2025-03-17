#include "pageguide.h"
#include "QtWidgets"

PageGuide::PageGuide(QWidget* parent) : QWidget(parent)
{
    QLabel* titrePageGuide = new QLabel(this);

    titrePageGuide->setText("<h1>Guide</h1>");

    QVBoxLayout* layoutVertical = new QVBoxLayout(this);

    layoutVertical->addWidget(titrePageGuide);
}
