#include "salle.h"
#include "editionsalle.h"

#include <QDebug>

Salle::Salle(QString nom, QWidget* parent) :
    QWidget(parent), nom(nom), editionPage(nullptr),
    baseDeDonnees(new CommunicationBaseDeDonnees(this))
{
    qDebug() << Q_FUNC_INFO << this << "nom" << nom;

    setWindowTitle("Salle");

    titre = new QLabel(QString(nom), this);
    QPixmap logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QLabel* labelLogoeLight      = new QLabel(this);
    QLabel* labelConsommation    = new QLabel(this);
    consommation                 = new QLabel(this);
    QLabel* segments             = new QLabel(this);
    QLabel* scenarios            = new QLabel(this);
    menuScenario                 = new QComboBox(this);
    menuSegment                  = new QComboBox(this);
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    QPushButton* boutonEdition   = new QPushButton("Éditer", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* entete = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);

    layout->addLayout(entete);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titre, Qt::AlignCenter);
    QString consommationText = consommation->text();
    labelConsommation->setText("Consommation d'énergie : " + consommationText);
    layout->addWidget(labelConsommation, 0, Qt::AlignHCenter);

    layout->addWidget(segments, 0, Qt::AlignLeft);
    layout->addWidget(menuSegment);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenario);
    layout->addWidget(boutonFermeture);
    layout->addWidget(boutonEdition);

    segments->setText("Segments :");

    scenarios->setText("Scénarios :");

    if(baseDeDonnees->connecter())
    {
        qDebug() << "Connexion réussi";
        chargerSegmentsDepuisBDD();
        chargerScenariosDepuisBDD();
        chargerConsommationDepuisBDD();
    }
    else
    {
        qDebug() << "Connexion échoué";
    }

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
    if(editionPage == nullptr)
        editionPage = new EditionSalle(this);
    editionPage->show();
}

void Salle::chargerScenariosDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare(
      "SELECT id_scenario, nom_scenario, intensite_scenario FROM scenario");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération des scénarios:"
                 << requete.lastError().text();
        return;
    }

    menuScenario->clear();

    while(requete.next())
    {
        QString idScenario        = requete.value(0).toString();
        QString nomScenario       = requete.value(1).toString();
        QString intensiteScenario = requete.value(2).toString();

        menuScenario->addItem("Scénario #" + idScenario + " - " + nomScenario +
                              " - " + intensiteScenario + " lux");
    }

    if(menuScenario->count() == 0)
    {
        menuScenario->addItem("Aucun scénario disponible");
    }
}

void Salle::chargerSegmentsDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_segment, ip_segment FROM segment");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération des segments:"
                 << requete.lastError().text();
        return;
    }

    menuSegment->clear();

    while(requete.next())
    {
        QString idSegment = requete.value(0).toString();
        QString ipSegment = requete.value(1).toString();

        menuSegment->addItem("Segment #" + idSegment + " - " +
                             "ip : " + ipSegment);
    }

    if(menuSegment->count() == 0)
    {
        menuSegment->addItem("Aucun segment disponible");
    }
}

void Salle::chargerConsommationDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT consommation FROM historique_consommation_segment");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération de la consommation:"
                 << requete.lastError().text();
        return;
    }

    if(requete.next())
    {
        QVariant consommationBDD = requete.value(0);

        if(consommationBDD.isValid())
        {
            consommation->setText(consommationBDD.toString());
        }
        else
        {
            qDebug() << "La consommation récupérée est invalide.";
        }
    }
    else
    {
        qDebug() << "Aucune donnée trouvée pour la consommation.";
    }
}
