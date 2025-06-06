#include "pageparametres.h"
#include "config.h"
#include <QDebug>

PageParametres::PageParametres(QWidget* parent) : QWidget(parent)
{
    QLabel* titreParametres = new QLabel(this);

    QFont police;
    police.setPointSize(TAILLE_POLICE);

    titreParametres->setFont(police);

    titreParametres->setText("<h1>Paramètres</h1><br>");

    QVBoxLayout* layoutVertical = new QVBoxLayout(this);

    QHBoxLayout* layoutEntete = new QHBoxLayout();

    layoutVertical->addLayout(layoutEntete);
    layoutVertical->addStretch();

    layoutEntete->addWidget(titreParametres);
}
