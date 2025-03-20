#include "pageaccueil.h"
#include <QDebug>

PageAccueil::PageAccueil(QWidget* parent) : QWidget(parent)
{
    QFrame* separateurPageAccueil = new QFrame(this);

    QLabel* titreScenarioActif = new QLabel(this);
    boutonGererScenarios       = new QPushButton(this);

    QLabel* texteScenarioActif     = new QLabel(this);
    QLabel* nomScenarioActif       = new QLabel(this);
    QLabel* intensiteScenarioActif = new QLabel(this);

    QLabel*      texteSelectionScenario           = new QLabel(this);
    QComboBox*   menuDeroulantScenarios           = new QComboBox(this);
    QPushButton* boutonConfirmerSelectionScenario = new QPushButton(this);

    QLabel* titreSegments = new QLabel(this);

    separateurPageAccueil->setFrameShape(QFrame::HLine);
    separateurPageAccueil->setFrameShadow(QFrame::Sunken);
    separateurPageAccueil->setObjectName("separateurPageAccueil");
    separateurPageAccueil->setFixedHeight(10);

    titreScenarioActif->setText("<h1>Scénario</h1>");
    boutonGererScenarios->setText("Gérer les scénarios");

    texteScenarioActif->setText("Scénario actif : ");
    nomScenarioActif->setText("PLACEHOLER");
    intensiteScenarioActif->setText("PLACEHOLDER");

    texteSelectionScenario->setText("Sélection scénario : ");
    menuDeroulantScenarios->addItem("PLACEHOLDER 1");
    menuDeroulantScenarios->addItem("PLACEHOLDER 2");
    boutonConfirmerSelectionScenario->setText("Valider");

    titreSegments->setText("<h1>Segments</h1>");

    QVBoxLayout* layoutVerticalPageAccueil = new QVBoxLayout(this);
    QHBoxLayout* layoutEnteteScenarioActif = new QHBoxLayout;
    QHBoxLayout* layoutScenarioActif       = new QHBoxLayout;
    QHBoxLayout* layoutSelectionScenario   = new QHBoxLayout;
    QHBoxLayout* layoutEnteteSegments      = new QHBoxLayout;
    layoutSegments                         = new QHBoxLayout;

    layoutVerticalPageAccueil->addLayout(layoutEnteteScenarioActif);
    layoutVerticalPageAccueil->addStretch();
    layoutVerticalPageAccueil->addLayout(layoutScenarioActif);
    layoutVerticalPageAccueil->addStretch();
    layoutVerticalPageAccueil->addLayout(layoutSelectionScenario);
    layoutVerticalPageAccueil->addWidget(separateurPageAccueil);
    layoutVerticalPageAccueil->addLayout(layoutEnteteSegments);
    layoutVerticalPageAccueil->addStretch();
    layoutVerticalPageAccueil->addLayout(layoutSegments);

    layoutEnteteScenarioActif->addWidget(titreScenarioActif);
    layoutEnteteScenarioActif->addWidget(boutonGererScenarios);

    layoutScenarioActif->addWidget(texteScenarioActif);
    layoutScenarioActif->addWidget(nomScenarioActif);
    layoutScenarioActif->addWidget(intensiteScenarioActif);

    layoutSelectionScenario->addWidget(texteSelectionScenario);
    layoutSelectionScenario->addWidget(menuDeroulantScenarios);
    layoutSelectionScenario->addWidget(boutonConfirmerSelectionScenario);

    layoutEnteteSegments->addWidget(titreSegments);
}

QPushButton* PageAccueil::getBoutonGererScenarios() const
{
    return boutonGererScenarios;
}

void PageAccueil::creerSegments(const int nombreScenarios)
{
    for(int i = 0; i < nombreScenarios; ++i)
    {
        BoiteSegment* segment = new BoiteSegment(i, this);
        listeSegments.append(segment);
    }
}

void PageAccueil::placerSegments()
{
    for(int i = 0; i < listeSegments.size(); ++i)
    {
        layoutSegments->addWidget(listeSegments[i]);
        listeSegments[i]->setConsommation(i);
    }
}
