/**
 * @file elightcontrol.cpp
 *
 * @brief Définition de la classe ELightControl
 * @author Soria Bonet Enzo
 * @version 1.0
 */

#include "elightcontrol.h"
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

    QWidget*        menuDeNavigation     = new QWidget(this);
    QStackedWidget* pageEmpilees         = new QStackedWidget(this);
    QWidget*        pageAccueil          = new QWidget(pageEmpilees);
    QWidget*        pageGestionScenarios = new QWidget(pageEmpilees);
    QWidget*        pageGuide            = new QWidget(pageEmpilees);
    QWidget*        pageParametres       = new QWidget(pageEmpilees);

    QLabel* labelLogoeLight  = new QLabel(menuDeNavigation);
    QString cheminRessources = "../" + QString(APPLICATION) + "/ressources/";
    QPixmap logoeLight(cheminRessources + "logoeLight.png");

    QPushButton* boutonAccueil          = new QPushButton(menuDeNavigation);
    QPushButton* boutonGestionScenarios = new QPushButton(menuDeNavigation);
    QPushButton* boutonGuide            = new QPushButton(menuDeNavigation);
    QPushButton* boutonParametres       = new QPushButton(menuDeNavigation);

    QFrame* separateurMenuNavigation = new QFrame(this);
    QFrame* separateurPageAccueil    = new QFrame(this);

    pageEmpilees->addWidget(pageAccueil);
    pageEmpilees->addWidget(pageGestionScenarios);
    pageEmpilees->addWidget(pageGuide);
    pageEmpilees->addWidget(pageParametres);

    separateurMenuNavigation->setFrameShape(QFrame::VLine);
    separateurMenuNavigation->setFrameShadow(QFrame::Sunken);
    separateurMenuNavigation->setObjectName("separateurMenuNavigation");
    separateurMenuNavigation->setFixedWidth(10);

    separateurPageAccueil->setFrameShape(QFrame::HLine);
    separateurPageAccueil->setFrameShadow(QFrame::Sunken);
    separateurPageAccueil->setObjectName("separateurPageAccueil");
    separateurPageAccueil->setFixedHeight(10);

    boutonAccueil->setText("Accueil");
    boutonAccueil->setFixedHeight(boutonAccueil->width());

    boutonGestionScenarios->setText("Scénarios");
    boutonGestionScenarios->setFixedHeight(boutonGestionScenarios->width());

    boutonGuide->setText("Guide");
    boutonGuide->setFixedHeight(boutonGuide->width());

    boutonParametres->setText("Paramètres");
    boutonParametres->setFixedHeight(boutonParametres->width());

    labelLogoeLight->setPixmap(logoeLight.scaled(200,
                                                 200,
                                                 Qt::KeepAspectRatio,
                                                 Qt::SmoothTransformation));

    QHBoxLayout* layoutHorizontalPrincipal = new QHBoxLayout(this);

    QVBoxLayout* layoutMenuDeNavigation = new QVBoxLayout(menuDeNavigation);
    QGridLayout* layoutBoutonsMenuDeNavigation = new QGridLayout;

    QVBoxLayout* layoutVerticalPageAccueil = new QVBoxLayout(pageAccueil);
    QHBoxLayout* layoutEnteteScenarioActif = new QHBoxLayout;
    QHBoxLayout* layoutScenarioActif       = new QHBoxLayout;
    QHBoxLayout* layoutSelectionScenario   = new QHBoxLayout;
    QHBoxLayout* layoutEnteteSegments      = new QHBoxLayout;
    QHBoxLayout* layoutSegments            = new QHBoxLayout;

    layoutHorizontalPrincipal->addWidget(menuDeNavigation);
    layoutHorizontalPrincipal->addWidget(separateurMenuNavigation);
    layoutHorizontalPrincipal->addWidget(pageEmpilees);

    layoutMenuDeNavigation->addWidget(labelLogoeLight);
    layoutMenuDeNavigation->addLayout(layoutBoutonsMenuDeNavigation);

    layoutBoutonsMenuDeNavigation->addWidget(boutonAccueil, GAUCHE, HAUT);
    layoutBoutonsMenuDeNavigation->addWidget(boutonGestionScenarios,
                                             DROITE,
                                             HAUT);
    layoutBoutonsMenuDeNavigation->addWidget(boutonGuide, GAUCHE, BAS);
    layoutBoutonsMenuDeNavigation->addWidget(boutonParametres, DROITE, BAS);

    layoutVerticalPageAccueil->addLayout(layoutEnteteScenarioActif);
    layoutVerticalPageAccueil->addLayout(layoutScenarioActif);
    layoutVerticalPageAccueil->addLayout(layoutSelectionScenario);
    layoutVerticalPageAccueil->addWidget(separateurPageAccueil);
    layoutVerticalPageAccueil->addLayout(layoutEnteteSegments);
    layoutVerticalPageAccueil->addLayout(layoutSegments);

    QFile styleFile(cheminRessources + "stylesheet.css");
    if(styleFile.open(QFile::ReadOnly))
    {
        QTextStream in(&styleFile);
        QString     style = in.readAll();
        this->setStyleSheet(style);
    }

    this->setFixedSize(1280, 720);
}

ELightControl::~ELightControl()
{
    qDebug() << Q_FUNC_INFO << this;
}
