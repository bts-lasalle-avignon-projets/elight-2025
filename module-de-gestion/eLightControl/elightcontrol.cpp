/**
 * @file elightcontrol.cpp
 *
 * @brief Définition de la classe ELightControl
 * @author Soria Bonet Enzo
 * @version 1.0
 */

#include "elightcontrol.h"
#include "menunavigation.h"
#include "pageaccueil.h"
#include "pagegestionscenario.h"
#include "pageguide.h"
#include "pageparametres.h"
#include <QDebug>

/**
 * @brief Constructeur de la classe ELightControl
 *
 * @fn ELightControl::ELightControl
 * @param parent L'adresse de l'objet parent, si nullptr ELightControl sera la
 * fenêtre principale de l'application
 */
ELightControl::ELightControl(QWidget* parent) : QWidget(parent)
{
    qDebug() << Q_FUNC_INFO << this;

    creerPages();
    gererNavigation();
    personnaliserFenetre();
}

ELightControl::~ELightControl()
{
    qDebug() << Q_FUNC_INFO << this;
}

void ELightControl::creerPages()
{
    // la pile de pages
    pageEmpilees = new QStackedWidget(this);

    // Le menu de navigation entre pages
    menuNavigation                   = new MenuNavigation(this);
    QFrame* separateurMenuNavigation = new QFrame(this);

    separateurMenuNavigation->setFrameShape(QFrame::VLine);
    separateurMenuNavigation->setFrameShadow(QFrame::Sunken);
    separateurMenuNavigation->setFixedWidth(10);

    // Les pages
    pageAccueil          = new PageAccueil(pageEmpilees);
    pageGestionScenarios = new PageGestionScenario(pageEmpilees);
    pageGuide            = new PageGuide(pageEmpilees);
    pageParametres       = new PageParametres(pageEmpilees);

    // Emplimenent des pages
    pageEmpilees->addWidget(pageAccueil);
    pageEmpilees->addWidget(pageGestionScenarios);
    pageEmpilees->addWidget(pageGuide);
    pageEmpilees->addWidget(pageParametres);

    // Positionnement dans la fenêtre
    QHBoxLayout* layoutHorizontalPrincipal = new QHBoxLayout(this);

    layoutHorizontalPrincipal->addWidget(menuNavigation);
    layoutHorizontalPrincipal->addWidget(separateurMenuNavigation);
    layoutHorizontalPrincipal->addWidget(pageEmpilees);
}

void ELightControl::gererNavigation()
{
    connect(menuNavigation->getBoutonAccueil(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::ACCUEIL);
            });

    connect(menuNavigation->getBoutonGestionScenarios(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::GESTION);
            });

    connect(menuNavigation->getBoutonGuide(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::GUIDE);
            });

    connect(menuNavigation->getBoutonParametres(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::PARAMETRES);
            });

    connect(pageAccueil->getBoutonGererScenarios(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::GESTION);
            });

    connect(pageGestionScenarios->getBoutonRetourGestionScenario(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::ACCUEIL);
            });

    connect(pageGuide->getBoutonRetourGuide(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::ACCUEIL);
            });

    connect(pageParametres->getBoutonRetourParametre(),
            &QPushButton::clicked,
            this,
            [=]
            {
                changerPage(Page::ACCUEIL);
            });
}

void ELightControl::personnaliserFenetre()
{
    setWindowTitle(QString(APPLICATION) + QString(" v") + QString(VERSION));

#ifdef RASPBERRY_PI
    showFullScreen();
#endif

    this->adjustSize();

    chargerFeuilleStyle();

    pageEmpilees->setCurrentIndex(Page::ACCUEIL);
}

void ELightControl::chargerFeuilleStyle()
{
    QString cheminRessources = "./" + QString(CHEMIN_RESSOURCES) + "/";
    QFile   styleFile(cheminRessources + QString(STYLE_APPLICATION));
    if(styleFile.open(QFile::ReadOnly))
    {
        QTextStream in(&styleFile);
        QString     style = in.readAll();
        this->setStyleSheet(style);
    }
}

void ELightControl::changerPage(Page page)
{
    pageEmpilees->setCurrentIndex(page);
}
