#include "pageparametres.h"

PageParametres::PageParametres(QWidget* parent) : QWidget(parent)
{
    QLabel* titreParametres = new QLabel(this);
    boutonRetourParametres  = new QPushButton(this);

    QLabel* texteNomDeSalle       = new QLabel(this);
    boiteNomDeSalle               = new QLineEdit(this);
    QPushButton* boutonValiderNom = new QPushButton(this);

    QLabel* texteIp              = new QLabel(this);
    boiteIp                      = new QLineEdit(this);
    QPushButton* boutonValiderIp = new QPushButton(this);

    titreParametres->setText("<h1>Paramètres</h1>");
    boutonRetourParametres->setText("Retour");

    texteNomDeSalle->setText("Nom de la salle : ");
    boiteNomDeSalle->setPlaceholderText("...");
    boutonValiderNom->setText("Valider");

    texteIp->setText("Adresse IP : ");
    boiteIp->setPlaceholderText("xxx.xxx.xxx.xxx");
    boutonValiderIp->setText("Valider");

    QVBoxLayout* layoutVertical = new QVBoxLayout(this);

    QHBoxLayout* layoutEntete = new QHBoxLayout();
    QHBoxLayout* layoutSalle  = new QHBoxLayout();
    QHBoxLayout* layoutIp     = new QHBoxLayout();

    layoutVertical->addLayout(layoutEntete);
    layoutVertical->addStretch();
    layoutVertical->addLayout(layoutSalle);
    layoutVertical->addLayout(layoutIp);

    layoutEntete->addWidget(titreParametres);
    layoutEntete->addWidget(boutonRetourParametres);

    layoutSalle->addWidget(texteNomDeSalle);
    layoutSalle->addWidget(boiteNomDeSalle);
    layoutSalle->addWidget(boutonValiderNom);

    layoutIp->addWidget(texteIp);
    layoutIp->addWidget(boiteIp);
    layoutIp->addWidget(boutonValiderIp);

    connect(boutonValiderNom, &QPushButton::clicked, this, [=] {
        validerParametreSalle();
    });
    connect(boutonValiderIp, &QPushButton::clicked, this, [=] {
        validerParametreIp();
    });
}

QPushButton* PageParametres::getBoutonRetourParametre() const
{
    return boutonRetourParametres;
}

void PageParametres::validerParametreSalle()
{
    if(!boiteNomDeSalle->text().isEmpty())
    {
        qDebug() << "Nom salle : " << boiteNomDeSalle->text();

        /**
         * @todo ajouter la requête sql CREATE/UPDATE NOM SALLE
         */
    }
}

void PageParametres::validerParametreIp()
{
    if(!boiteIp->text().isEmpty())
    {
        qDebug() << "Ip boitier : " << boiteIp->text();

        /**
         * @todo ajouter la requête sql CREATE/UPDATE IP
         */
    }
}
