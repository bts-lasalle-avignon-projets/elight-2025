#include "pageaccueil.h"
#include "boitesegment.h"
#include <QDebug>

PageAccueil::PageAccueil(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    qDebug() << Q_FUNC_INFO << this;

    QFrame* separateurPageAccueil = new QFrame(this);

    QLabel* titreScenarioActif = new QLabel(this);
    boutonGererScenarios       = new QPushButton(this);

    QLabel* texteScenarioActif     = new QLabel(this);
    QLabel* nomScenarioActif       = new QLabel(this);
    QLabel* intensiteScenarioActif = new QLabel(this);

    QLabel* texteSelectionScenario                = new QLabel(this);
    menuDeroulantScenarios                        = new QComboBox(this);
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
    layoutSegments                         = new QGridLayout;

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

    if(baseDeDonnees.connecter())
    {
        chargerSegmentsDepuisBDD();
        chargerScenariosDepuisBDD();
    }
    else
    {
        qDebug() << "Erreur: Impossible de se connecter à la base de données";
    }
}

PageAccueil::~PageAccueil()
{
    qDebug() << Q_FUNC_INFO << this;
}

QPushButton* PageAccueil::getBoutonGererScenarios() const
{
    return boutonGererScenarios;
}

void PageAccueil::chargerScenariosDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_scenario, nom_scenario FROM scenario");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération des scénarios:"
                 << requete.lastError().text();
        return;
    }

    menuDeroulantScenarios->clear();

    while(requete.next())
    {
        int     idScenario  = requete.value(0).toInt();
        QString nomScenario = requete.value(1).toString();

        menuDeroulantScenarios->addItem(nomScenario, idScenario);
    }

    if(menuDeroulantScenarios->count() == 0)
    {
        menuDeroulantScenarios->addItem("Aucun scénario disponible");
    }
}

void PageAccueil::chargerSegmentsDepuisBDD()
{
    QString nomSalle = "B20"; // temporaire

    bool resultats = false;

    QSqlQuery requete;

    requete.prepare("SELECT segment.id_segment FROM segment "
                    "JOIN salle ON segment.id_salle = salle.id_salle "
                    "WHERE salle.nom_salle = :nom_salle");

    requete.bindValue(":nom_salle", nomSalle);

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération des segments:"
                 << requete.lastError().text();
        return;
    }

    listeSegments.clear();

    int ligne   = 0;
    int colonne = 0;

    while(requete.next())
    {
        resultats = true;

        int idSegment = requete.value(0).toInt();

        QLabel* labelSegmentId =
          new QLabel(QString("<h2>Segment %1</h2>").arg(idSegment));
        labelSegmentId->setAlignment(Qt::AlignCenter);

        BoiteSegment* segment = new BoiteSegment(idSegment, this);
        listeSegments.append(segment);

        layoutSegments->addWidget(labelSegmentId, ligne, colonne);
        layoutSegments->addWidget(segment, ligne + 1, colonne);
        segment->setConsommation(0);

        colonne++;
        if(colonne >= COLONNES_MAX)
        {
            colonne = 0;
            ligne += 2;
        }
    }

    if(!resultats)
    {
        QLabel* labelAucunSegment =
          new QLabel(QString("<h2>Aucun segment enregistré</h2>"));
        labelAucunSegment->setAlignment(Qt::AlignCenter);
        layoutSegments->addWidget(labelAucunSegment);
    }
}
