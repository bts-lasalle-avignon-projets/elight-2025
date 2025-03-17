#include "menunavigation.h"
#include "elightcontrol.h"

MenuNavigation::MenuNavigation(QWidget* parent) : QWidget(parent)
{
    QLabel* labelLogoeLight  = new QLabel(this);
    QString cheminRessources = "./" + QString(CHEMIN_RESSOURCES) + "/";
    QPixmap logoeLight(cheminRessources + QString(LOGO_APPLICATION));

    boutonAccueil          = new QPushButton(this);
    boutonGestionScenarios = new QPushButton(this);
    boutonGuide            = new QPushButton(this);
    boutonParametres       = new QPushButton(this);

    boutonAccueil->setText("Accueil");
    boutonAccueil->setFixedHeight(boutonAccueil->width());

    boutonGestionScenarios->setText("Scénarios");
    boutonGestionScenarios->setFixedHeight(boutonGestionScenarios->width());

    boutonGuide->setText("Guide");
    boutonGuide->setFixedHeight(boutonGuide->width());

    boutonParametres->setText("Paramètres");
    boutonParametres->setFixedHeight(boutonParametres->width());

    labelLogoeLight->setPixmap(logoeLight.scaled(LARGEUR_LOGO,
                                                 HAUTEUR_LOGO,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));

    QVBoxLayout* layoutMenuDeNavigation        = new QVBoxLayout(this);
    QGridLayout* layoutBoutonsMenuDeNavigation = new QGridLayout;

    layoutMenuDeNavigation->addWidget(labelLogoeLight);
    layoutMenuDeNavigation->addStretch();
    layoutMenuDeNavigation->addLayout(layoutBoutonsMenuDeNavigation);

    layoutBoutonsMenuDeNavigation->addWidget(boutonAccueil, HAUT, GAUCHE);
    layoutBoutonsMenuDeNavigation->addWidget(boutonGestionScenarios,
                                             HAUT,
                                             DROITE);
    layoutBoutonsMenuDeNavigation->addWidget(boutonGuide, BAS, GAUCHE);
    layoutBoutonsMenuDeNavigation->addWidget(boutonParametres, BAS, DROITE);
}

QPushButton* MenuNavigation::getBoutonAccueil() const
{
    return boutonAccueil;
}

QPushButton* MenuNavigation::getBoutonGestionScenarios() const
{
    return boutonGestionScenarios;
}

QPushButton* MenuNavigation::getBoutonGuide() const
{
    return boutonGuide;
}

QPushButton* MenuNavigation::getBoutonParametres() const
{
    return boutonParametres;
}
