#include "pageparametres.h"
#include <QtWidgets>

PageParametres::PageParametres(QWidget* parent) : QWidget(parent)
{
    QLabel* titreParametres = new QLabel(this);

    QLabel*      texteNomDeSalle  = new QLabel(this);
    QLineEdit*   boiteNomDeSalle  = new QLineEdit(this);
    QPushButton* boutonValiderNom = new QPushButton(this);

    QLabel*      texteIp         = new QLabel(this);
    QLineEdit*   boiteIp         = new QLineEdit(this);
    QPushButton* boutonValiderIp = new QPushButton(this);

    titreParametres->setText("<h1>Paramètres</h1>");

    texteNomDeSalle->setText("Nom de la salle : ");
    boiteNomDeSalle->setPlaceholderText("...");
    boutonValiderNom->setText("Valider");

    texteIp->setText("Adresse IP : ");
    boiteIp->setPlaceholderText("...");
    boutonValiderIp->setText("Valider");

    QVBoxLayout* layoutVertical = new QVBoxLayout(this);

    QHBoxLayout* layoutEntete = new QHBoxLayout();
    QHBoxLayout* layoutSalle  = new QHBoxLayout();
    QHBoxLayout* layoutIp     = new QHBoxLayout();

    layoutVertical->addLayout(layoutEntete);
    layoutVertical->addLayout(layoutSalle);
    layoutVertical->addLayout(layoutIp);

    layoutEntete->addWidget(titreParametres);

    layoutSalle->addWidget(texteNomDeSalle);
    layoutSalle->addWidget(boiteNomDeSalle);
    layoutSalle->addWidget(boutonValiderNom);

    layoutIp->addWidget(texteIp);
    layoutIp->addWidget(boiteIp);
    layoutIp->addWidget(boutonValiderIp);
}
