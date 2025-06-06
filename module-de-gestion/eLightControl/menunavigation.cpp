#include "menunavigation.h"
#include "config.h"
#include <QDebug>

MenuNavigation::MenuNavigation(QWidget* parent) : QWidget(parent)
{
    QLabel* labelLogoeLight  = new QLabel(this);
    QString cheminRessources = "./" + QString(CHEMIN_RESSOURCES) + "/";
    QPixmap logoeLight(cheminRessources + QString(LOGO_APPLICATION));
    QIcon   iconeAccueil(cheminRessources + QString(ICONE_ACCUEIL));
    QIcon   iconeGestion(cheminRessources + QString(ICONE_GESTION));
    QIcon   iconeGuide(cheminRessources + QString(ICONE_GUIDE));
    QIcon   iconeParametre(cheminRessources + QString(ICONE_PARAMETRE));

    boutonAccueil          = new QPushButton(this);
    boutonGestionScenarios = new QPushButton(this);
    boutonGuide            = new QPushButton(this);
    boutonParametres       = new QPushButton(this);

    QFont police;
    police.setPointSize(TAILLE_POLICE);

    boutonAccueil->setFont(police);
    boutonGestionScenarios->setFont(police);
    boutonGuide->setFont(police);
    boutonParametres->setFont(police);

    boutonAccueil->setIcon(iconeAccueil);
    boutonAccueil->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS, HAUTEUR_ICONES_BOUTONS));
    boutonAccueil->setFixedSize(LARGEUR_ICONES_BOUTONS + PADDING_ICONES,
                                HAUTEUR_ICONES_BOUTONS + PADDING_ICONES);

    boutonGestionScenarios->setIcon(iconeGestion);
    boutonGestionScenarios->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS, HAUTEUR_ICONES_BOUTONS));
    boutonGestionScenarios->setFixedSize(
      LARGEUR_ICONES_BOUTONS + PADDING_ICONES,
      HAUTEUR_ICONES_BOUTONS + PADDING_ICONES);

    boutonGuide->setIcon(iconeGuide);
    boutonGuide->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS, HAUTEUR_ICONES_BOUTONS));
    boutonGuide->setFixedSize(LARGEUR_ICONES_BOUTONS + PADDING_ICONES,
                              HAUTEUR_ICONES_BOUTONS + PADDING_ICONES);

    boutonParametres->setIcon(iconeParametre);
    boutonParametres->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS, HAUTEUR_ICONES_BOUTONS));
    boutonParametres->setFixedSize(LARGEUR_ICONES_BOUTONS + PADDING_ICONES,
                                   HAUTEUR_ICONES_BOUTONS + PADDING_ICONES);

    labelLogoeLight->setPixmap(logoeLight.scaled(LARGEUR_LOGO_APPLICATION,
                                                 HAUTEUR_LOGO_APPLICATION,
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
