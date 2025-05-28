#include "salle.h"
#include "editionsalle.h"

#include <QDebug>

Salle::Salle(QString nom, QWidget* parent) :
    QWidget(parent), nom(nom), idSalle(-1), editionPage(nullptr),
    baseDeDonnees(CommunicationBaseDeDonnees::creerInstance())
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
    menuScenario                 = new QListWidget(this);
    menuSegment                  = new QListWidget(this);
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    QPushButton* boutonEdition   = new QPushButton("Éditer", this);

    QVBoxLayout* layout      = new QVBoxLayout(this);
    QHBoxLayout* entete      = new QHBoxLayout;
    QHBoxLayout* layoutConso = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);

    layout->addLayout(entete);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titre, Qt::AlignCenter);
    QString consommationText = consommation->text();
    labelConsommation->setText("Consommation d'énergie : " + consommationText);
    layoutConso->addWidget(labelConsommation);
    layoutConso->addWidget(consommation);
    layout->addLayout(layoutConso, Qt::AlignHCenter);

    layout->addWidget(segments, 0, Qt::AlignLeft);
    layout->addWidget(menuSegment);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenario);
    layout->addWidget(boutonFermeture);
    layout->addWidget(boutonEdition);

    segments->setText("Segments :");
    scenarios->setText("Scénario actif :");

    if(baseDeDonnees->connecter())
    {
        QSqlQuery requete;
        requete.prepare(
          "SELECT id_salle FROM salle WHERE nom_salle = :nomSalle");
        requete.bindValue(":nomSalle", nom);

        if(requete.exec() && requete.next())
        {
            idSalle = requete.value(COLONNE_ID_SALLE).toInt();
        }
        else
        {
            qDebug() << "Erreur lors de la récupération de l'id_salle pour "
                        "la salle "
                     << nom;
        }

        chargerSegmentsDepuisBDD();
        chargerScenariosDepuisBDD();
        chargerConsommationDepuisBDD();
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
    consommation->setStyleSheet(
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
    if(editionPage != nullptr)
        delete editionPage;
    CommunicationBaseDeDonnees::detruireInstance();
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
    Q_UNUSED(event)
    qDebug() << Q_FUNC_INFO << this << "nom" << nom;

    rechargerDonnees();
}

void Salle::rechargerDonnees()
{
    chargerSegmentsDepuisBDD();

    chargerScenariosDepuisBDD();

    chargerConsommationDepuisBDD();
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
      "SELECT id_scenario, nom_scenario, intensite_scenario FROM scenario "
      "WHERE id_scenario IN (SELECT id_scenario FROM segment WHERE "
      "id_salle = "
      ":idSalle)");

    requete.bindValue(":idSalle", idSalle);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    menuScenario->clear();

    while(requete.next())
    {
        QString idScenario  = requete.value(COLONNE_ID_SCENARIO).toString();
        QString nomScenario = requete.value(COLONNE_NOM_SCENARIO).toString();
        QString intensiteScenario =
          requete.value(COLONNE_INTENSITE_SCENARIO).toString();

        menuScenario->addItem("Scénario #" + idScenario + " - " + nomScenario +
                              " - " + intensiteScenario + " lux");
    }

    if(menuScenario->count() == 0)
    {
        menuScenario->addItem("Aucun scénario actif");
    }
}

void Salle::chargerSegmentsDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare(
      "SELECT s.id_segment, s.ip_segment, "
      "(SELECT COUNT(*) FROM historique_consommation_segment h "
      " WHERE h.id_segment = s.id_segment) AS consommation_existe "
      "FROM segment s WHERE s.id_salle = :idSalle");

    requete.bindValue(":idSalle", idSalle);

    if(!requete.exec())
    {
        qDebug() << "Erreur SQL" << requete.lastError().text();
        return;
    }

    menuSegment->clear();
    while(requete.next())
    {
        QString idSegment = requete.value(COLONNE_ID_SEGMENT).toString();
        QString ipSegment = requete.value(COLONNE_IP_SEGMENT).toString();
        bool    consommationExiste =
          requete.value(CHAMP_CONSOMMATION_EXISTE).toInt() > 0;

        QListWidgetItem* item =
          new QListWidgetItem("Segment #" + idSegment + " - ip : " + ipSegment);

        if(consommationExiste)
        {
            item->setBackground(QBrush(QColor("#70eb65")));
        }
        else
        {
            item->setBackground(QBrush(QColor("#eb6565")));
        }

        menuSegment->addItem(item);
    }

    if(menuSegment->count() == 0)
    {
        menuSegment->addItem("Aucun segment disponible");
    }
}

void Salle::chargerConsommationDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT consommation FROM historique_consommation_segment "
                    "WHERE id_segment IN (SELECT id_segment FROM segment WHERE "
                    "id_salle = :idSalle)");

    requete.bindValue(":idSalle", idSalle);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    double consommationTotale = 0.0;

    while(requete.next())
    {
        QVariant consommationBDD = requete.value(COLONNE_CONSOMMATION_BDD);

        if(consommationBDD.isValid())
        {
            consommationTotale += consommationBDD.toDouble();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "La consommation récupérée est invalide";
        }
    }

    if(consommationTotale > 0)
    {
        consommation->setText(QString::number(consommationTotale, 'f', 2) +
                              " kWh");
    }
    else
    {
        consommation->setText(QString::number(consommationTotale, '0', 2) +
                              " kWh");
        qDebug() << Q_FUNC_INFO << "Aucune donnée trouvée pour la consommation";
    }
}
