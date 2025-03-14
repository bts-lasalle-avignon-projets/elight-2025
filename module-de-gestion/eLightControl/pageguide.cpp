#include "pageguide.h"
#include "QtWidgets"

PageGuide::PageGuide(QWidget* parent) : QWidget(parent)
{
    QLabel* titrePageGuide = new QLabel(this);

    /*QLabel* labelImageGuide = new QLabel(this);
    QString cheminRessources = "../eLightControl/ressources/";
    QPixmap imageGuide(cheminRessources + "guide.png");*/

    titrePageGuide->setText("<h1>Guide</h1>");

    // labelImageGuide->setPixmap(imageGuide);

    QVBoxLayout* layoutVertical = new QVBoxLayout(this);

    layoutVertical->addWidget(titrePageGuide);
    // layoutVertical->addWidget(labelImageGuide);
}
