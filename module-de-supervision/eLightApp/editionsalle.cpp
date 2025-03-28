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

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* entete = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titreEdition, Qt::AlignBaseline);

    layout->addLayout(entete);
    layout->addWidget(segments, 0, Qt::AlignLeft);
    layout->addWidget(menuSegment);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenario);
    layout->addWidget(boutonFermeture);

    segments->setText("Segments :");
    menuSegment->addItem("Ajouter un segment :");
    menuSegment->addItem("Modifier un segment :");
    menuSegment->addItem("Supprimer un segment :");

    scenarios->setText("Scénarios :");
    menuScenario->addItem("Ajouter un scénario :");
    menuScenario->addItem("Modifier un scénario :");
    menuScenario->addItem("Supprimer un scénario :");

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &EditionSalle::fermerFenetre);

    this->setStyleSheet("background-color: #FFFFFF;");
    titreEdition->setStyleSheet("font-weight: 900; font-size: 50px;");
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
