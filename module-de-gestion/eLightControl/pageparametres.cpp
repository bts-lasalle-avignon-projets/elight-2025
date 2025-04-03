#include "pageparametres.h"

PageParametres::PageParametres(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
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
    QString nomSalle        = "B20"; // Temporaire
    QString nouveauNomSalle = boiteNomDeSalle->text();

    if(!nouveauNomSalle.isEmpty())
    {
        QSqlQuery requete;
        requete.prepare("UPDATE salle SET nom_salle = :nouveau_nom_salle WHERE "
                        "nom_salle = :nom_salle");
        requete.bindValue(":nouveau_nom_salle", nouveauNomSalle);
        requete.bindValue(":nom_salle", nomSalle);

        if(!requete.exec())
        {
            qDebug() << "Erreur lors de la modification du nom de la salle:"
                     << requete.lastError().text();
        }
        else
        {
            qDebug() << "Salle modifiée avec succès ! "
                     << "Nom salle : " << nomSalle;
        }
    }
}

void PageParametres::validerParametreIp()
{
    QString nouvelleIp = boiteIp->text();

    if(!nouvelleIp.isEmpty())
    {
        QString nomSalle = "B20"; // Temporaire

        QSqlQuery requete;
        requete.prepare("UPDATE salle SET ip_boitier = :nouvelle_ip WHERE "
                        "nom_salle = :nom_salle");
        requete.bindValue(":nouvelle_ip", nouvelleIp);
        requete.bindValue(":nom_salle", nomSalle);

        if(!requete.exec())
        {
            qDebug() << "Erreur lors de la modification de l'ip de la salle:"
                     << requete.lastError().text();
        }
        else
        {
            qDebug() << "Salle modifiée avec succès ! "
                     << "Ip boitier : " << nouvelleIp;
        }
    }
}
