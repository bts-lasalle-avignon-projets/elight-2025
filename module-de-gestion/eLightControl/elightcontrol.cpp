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

    setWindowTitle(QString(APPLICATION) + QString(" v") + QString(VERSION));

#ifdef RASPBERRY_PI
    showFullScreen();
#endif

    pageEmpilees                                  = new QStackedWidget(this);
    MenuNavigation*      menuNavigation           = new MenuNavigation(this);
    QFrame*              separateurMenuNavigation = new QFrame(this);
    PageAccueil*         pageAccueil = new PageAccueil(pageEmpilees);
    PageGestionScenario* pageGestionScenarios =
      new PageGestionScenario(pageEmpilees);
    PageGuide*      pageGuide      = new PageGuide(pageEmpilees);
    PageParametres* pageParametres = new PageParametres(pageEmpilees);

    separateurMenuNavigation->setFrameShape(QFrame::VLine);
    separateurMenuNavigation->setFrameShadow(QFrame::Sunken);
    separateurMenuNavigation->setObjectName("separateurMenuNavigation");
    separateurMenuNavigation->setFixedWidth(10);

    pageEmpilees->addWidget(pageAccueil);
    pageEmpilees->addWidget(pageGestionScenarios);
    pageEmpilees->addWidget(pageGuide);
    pageEmpilees->addWidget(pageParametres);

    QHBoxLayout* layoutHorizontalPrincipal = new QHBoxLayout(this);

    layoutHorizontalPrincipal->addWidget(menuNavigation);
    layoutHorizontalPrincipal->addWidget(separateurMenuNavigation);
    layoutHorizontalPrincipal->addWidget(pageEmpilees);

    pageEmpilees->setCurrentIndex(0);
    this->adjustSize();

    chargerFeuilleStyle();
}

ELightControl::~ELightControl()
{
    qDebug() << Q_FUNC_INFO << this;
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
