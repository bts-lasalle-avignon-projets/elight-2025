#include "interfacehommemachine.h"
#include "menunavigation.h"
#include "pageaccueil.h"
#include "pagegestionscenario.h"
#include "pageguide.h"
#include "pageparametres.h"

InterfaceHommeMachine::InterfaceHommeMachine(QWidget* parent) : QWidget(parent)
{
    pageEmpilees                                  = new QStackedWidget(this);
    MenuNavigation*      menuNavigation           = new MenuNavigation(this);
    QFrame*              separateurMenuNavigation = new QFrame(this);
    PageAccueil*         pageAccueil = new PageAccueil(pageEmpilees);
    PageGestionScenario* pageGestionScenarios =
      new PageGestionScenario(pageEmpilees);
    PageGuide*      pageGuide      = new PageGuide(pageEmpilees);
    PageParametres* pageParametres = new PageParametres(pageEmpilees);

    /* ---------- ----------- */

    separateurMenuNavigation->setFrameShape(QFrame::VLine);
    separateurMenuNavigation->setFrameShadow(QFrame::Sunken);
    separateurMenuNavigation->setObjectName("separateurMenuNavigation");
    separateurMenuNavigation->setFixedWidth(10);

    pageEmpilees->addWidget(pageAccueil);
    pageEmpilees->addWidget(pageGestionScenarios);
    pageEmpilees->addWidget(pageGuide);
    pageEmpilees->addWidget(pageParametres);

    /* ---------- ----------- */

    QHBoxLayout* layoutHorizontalPrincipal = new QHBoxLayout(this);

    /* ---------- ----------- */

    layoutHorizontalPrincipal->addWidget(menuNavigation);
    layoutHorizontalPrincipal->addWidget(separateurMenuNavigation);
    layoutHorizontalPrincipal->addWidget(pageEmpilees);

    /* ---------- ----------- */

    pageEmpilees->setCurrentIndex(3);
    this->adjustSize();
}
