#include "salle.h"
#include "editionsalle.h"

#include <QDebug>

Salle::Salle(QString nom, QWidget* parent) : QWidget(parent), nom(nom)
{
    qDebug() << Q_FUNC_INFO << this << "nom" << nom;

    setWindowTitle("Salle");

    titre = new QLabel(QString(nom), this);
    QPixmap logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QLabel* labelLogoeLight   = new QLabel(this);
    QLabel* labelConsommation = new QLabel("Consommation d'énergie : ", this);
    consommation              = new QLabel("XXX", this);
    QLabel*      segments     = new QLabel(this);
    QLabel*      scenarios    = new QLabel(this);
    QComboBox*   menuScenario = new QComboBox(this);
    QComboBox*   menuSegment  = new QComboBox(this);
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    QPushButton* boutonEdition   = new QPushButton("Éditer", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* entete = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);

    layout->addLayout(entete);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titre, Qt::AlignCenter);
    layout->addWidget(labelConsommation, 0, Qt::AlignHCenter);

    layout->addWidget(segments, 0, Qt::AlignLeft);
    layout->addWidget(menuSegment);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenario);
    layout->addWidget(boutonFermeture);
    layout->addWidget(boutonEdition);

    segments->setText("Segments :");
    menuSegment->addItem("Segment 1");
    menuSegment->addItem("Segment 2");

    scenarios->setText("Scénarios :");
    menuScenario->addItem("Scénario 1");
    menuScenario->addItem("Scénario 2");

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &Salle::fermerFenetre);
    connect(boutonEdition, &QPushButton::clicked, this, &Salle::editerSalle);

    this->setStyleSheet("background-color: #FFFFFF;");
    titre->setStyleSheet("font-weight: 900; font-size: 50px;");
    labelConsommation->setStyleSheet(
      "border: 1px solid black; background-color: #FFFF33;");
#ifdef RASPBERRY_PI
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Dialog); // Ajouter Qt::WindowStaysOnTopHint*/
#else
    setWindowFlags(Qt::Dialog);
#endif

    setWindowModality(Qt::WindowModal);
}

Salle::~Salle()
{
    qDebug() << Q_FUNC_INFO << this << "nom" << nom;
}

/**
 * @brief S'exécute à chaque fois que la fenêtre est affichée
 *
 * @fn Historique::showEvent
 *
 */
void Salle::showEvent(QShowEvent* event)
{
    qDebug() << Q_FUNC_INFO << this << "nom" << nom;
}

void Salle::fermerFenetre()
{
    qDebug() << Q_FUNC_INFO << this << "nom" << nom;
    this->close();
}

void Salle::editerSalle()
{
}
