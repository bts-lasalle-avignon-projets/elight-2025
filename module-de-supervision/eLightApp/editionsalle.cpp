#include "editionsalle.h"

#include <QDebug>

EditionSalle::EditionSalle(QWidget* parent) : QWidget(parent)
{
    qDebug() << Q_FUNC_INFO << this;

    setWindowTitle(QString(TITRE_FENETRE_EDITION));

    QPixmap      logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QLabel*      titreEdition    = new QLabel("Édition", this);
    QLabel*      labelLogoeLight = new QLabel(this);
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    QLabel*      segments        = new QLabel(this);
    QLabel*      scenarios       = new QLabel(this);
    QComboBox*   menuScenario    = new QComboBox(this);
    QComboBox*   menuSegment     = new QComboBox(this);
    QLabel*      ajoutSegment    = new QLabel(this);
    QLabel*      modifSegment    = new QLabel(this);
    QLabel*      supprSegment    = new QLabel(this);
    QLabel*      ajoutScenario   = new QLabel(this);
    QLabel*      modifScenario   = new QLabel(this);
    QLabel*      supprScenario   = new QLabel(this);

    QVBoxLayout* layout          = new QVBoxLayout(this);
    QHBoxLayout* entete          = new QHBoxLayout;
    QGridLayout* editionSegment  = new QGridLayout;
    QGridLayout* editionScenario = new QGridLayout;

    labelLogoeLight->setPixmap(logoeLight);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titreEdition, Qt::AlignBaseline);

    ajoutSegment->setText("Ajouter");
    editionSegment->addWidget(ajoutSegment, 0, 0);
    modifSegment->setText("Modifier");
    editionSegment->addWidget(modifSegment, 0, 1);
    supprSegment->setText("Supprimer");
    editionSegment->addWidget(supprSegment, 0, 2);

    ajoutScenario->setText("Ajouter");
    editionScenario->addWidget(ajoutScenario, 0, 0);
    modifScenario->setText("Modifier");
    editionScenario->addWidget(modifScenario, 0, 1);
    supprScenario->setText("Supprimer");
    editionScenario->addWidget(supprScenario, 0, 2);

    layout->addLayout(entete);
    layout->addWidget(segments, 0, Qt::AlignLeft);
    layout->addWidget(menuSegment);
    layout->addLayout(editionSegment);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenario);
    layout->addLayout(editionScenario);
    layout->addWidget(boutonFermeture);

    segments->setText("Segments :");
    menuSegment->addItem("Segment 1");
    menuSegment->addItem("Segment 2");
    menuSegment->addItem("..."); // ajouter un scenario

    scenarios->setText("Scénarios :");
    menuScenario->addItem("Scénario 1");
    menuScenario->addItem("Scénario 2");
    menuScenario->addItem("..."); // ajouter un scenario

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &EditionSalle::fermerFenetre);

    this->setStyleSheet("background-color: #FFFFFF;");
    titreEdition->setStyleSheet("font-weight: 900; font-size: 50px;");
    ajoutSegment->setStyleSheet("border: 1px solid black;");
    modifSegment->setStyleSheet("border: 1px solid black;");
    supprSegment->setStyleSheet("border: 1px solid black;");
    ajoutScenario->setStyleSheet("border: 1px solid black;");
    modifScenario->setStyleSheet("border: 1px solid black;");
    supprScenario->setStyleSheet("border: 1px solid black;");
#ifdef RASPBERRY_PI
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Dialog); // Ajouter Qt::WindowStaysOnTopHint*/
#else
    setWindowFlags(Qt::Dialog);
#endif

    setWindowModality(Qt::WindowModal);
}

void EditionSalle::fermerFenetre()
{
    this->close();
}
