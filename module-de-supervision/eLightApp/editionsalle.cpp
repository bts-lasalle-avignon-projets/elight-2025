#include "editionsalle.h"

#include <QDebug>

EditionSalle::EditionSalle(QWidget* parent) :
    QWidget(parent), baseDeDonnees(new CommunicationBaseDeDonnees(this))
{
    qDebug() << Q_FUNC_INFO << this;

    setWindowTitle(QString(TITRE_FENETRE_EDITION));

    QPixmap      logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QLabel*      titreEdition    = new QLabel("Édition", this);
    QLabel*      labelLogoeLight = new QLabel(this);
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    QLabel*      segments        = new QLabel(this);
    QLabel*      scenarios       = new QLabel(this);
    menuScenarios                = new QComboBox(this);
    menuSegments                 = new QComboBox(this);
    ajoutIPSegment               = new QLineEdit(this);
    ajoutScenario                = new QLineEdit(this);
    ajoutIntensiteScenario       = new QLineEdit(this);
    QPushButton* validerSegment  = new QPushButton("Ajouter", this);
    QPushButton* modifSegment    = new QPushButton("Modifier", this);
    QPushButton* supprSegment    = new QPushButton("Supprimer", this);
    QPushButton* validerScenario = new QPushButton("Ajouter", this);
    QPushButton* modifScenario   = new QPushButton("Modifier", this);
    QPushButton* supprScenario   = new QPushButton("Supprimer", this);

    QVBoxLayout* layout          = new QVBoxLayout(this);
    QHBoxLayout* entete          = new QHBoxLayout;
    QGridLayout* editionSegment  = new QGridLayout;
    QGridLayout* editionScenario = new QGridLayout;

    labelLogoeLight->setPixmap(logoeLight);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titreEdition, Qt::AlignBaseline);

    editionSegment->addWidget(modifSegment, 0, 0);
    editionSegment->addWidget(supprSegment, 0, 1);
    editionSegment->addWidget(ajoutIPSegment, 1, 0);
    ajoutIPSegment->setPlaceholderText("xxx.xxx.xxx.xxx");
    editionSegment->addWidget(validerSegment, 1, 1);

    editionScenario->addWidget(modifScenario, 0, 0);
    editionScenario->addWidget(supprScenario, 0, 1);
    editionScenario->addWidget(ajoutScenario, 1, 0);
    ajoutScenario->setPlaceholderText("nom du scénario");
    editionScenario->addWidget(ajoutIntensiteScenario, 1, 1);
    ajoutIntensiteScenario->setPlaceholderText("intensité (en lux)");
    editionScenario->addWidget(validerScenario, 1, 2);

    layout->addLayout(entete);
    layout->addWidget(segments, 0, Qt::AlignLeft);
    layout->addWidget(menuSegments);
    layout->addLayout(editionSegment);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenarios);
    layout->addLayout(editionScenario);
    layout->addWidget(boutonFermeture);

    segments->setText("Segments :");

    scenarios->setText("Scénarios :");

    if(baseDeDonnees->connecter())
    {
        qDebug() << "Connexion réussi";
        chargerScenariosDepuisBDD();
        chargerSegmentsDepuisBDD();
    }
    else
    {
        qDebug() << "Connexion échoué";
    }

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &EditionSalle::fermerFenetre);
    connect(validerSegment,
            &QPushButton::clicked,
            this,
            &EditionSalle::ajouterSegmentsBDD);
    connect(supprSegment,
            &QPushButton::clicked,
            this,
            &EditionSalle::supprimerSegmentsBDD);
    connect(modifSegment,
            &QPushButton::clicked,
            this,
            &EditionSalle::modifierSegmentsBDD);
    connect(validerScenario,
            &QPushButton::clicked,
            this,
            &EditionSalle::ajouterScenariosBDD);
    connect(modifScenario,
            &QPushButton::clicked,
            this,
            &EditionSalle::modifierScenariosBDD);
    connect(supprScenario,
            &QPushButton::clicked,
            this,
            &EditionSalle::supprimerScenariosBDD);

    this->setStyleSheet("background-color: #FFFFFF;");
    titreEdition->setStyleSheet("font-weight: 900; font-size: 50px;");
    validerSegment->setStyleSheet("border: 1px solid black;");
    modifSegment->setStyleSheet("border: 1px solid black;");
    supprSegment->setStyleSheet("border: 1px solid black;");
    validerScenario->setStyleSheet("border: 1px solid black;");
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

void EditionSalle::chargerScenariosDepuisBDD()
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

    menuScenarios->clear();

    while(requete.next())
    {
        QString idScenario        = requete.value(0).toString();
        QString nomScenario       = requete.value(1).toString();
        QString intensiteScenario = requete.value(2).toString();

        menuScenarios->addItem("Scénario #" + idScenario + " - " + nomScenario +
                               " - " + intensiteScenario + " lux");
    }

    if(menuScenarios->count() == 0)
    {
        menuScenarios->addItem("Aucun scénario disponible");
    }
}

void EditionSalle::chargerSegmentsDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_segment, ip_segment FROM segment");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération des segments:"
                 << requete.lastError().text();
        return;
    }

    menuSegments->clear();

    while(requete.next())
    {
        QString idSegment = requete.value(0).toString();
        QString ipSegment = requete.value(1).toString();

        menuSegments->addItem("Segment #" + idSegment + " - " +
                              "ip : " + ipSegment);
    }

    if(menuSegments->count() == 0)
    {
        menuSegments->addItem("Aucun segment disponible");
    }
}

void EditionSalle::supprimerSegmentsBDD()
{
    QString segmentChoisi = menuSegments->currentText();

    QStringList partiesSegment = segmentChoisi.split(" - ");
    QString     idSegment = partiesSegment.at(0).split("#").at(1).trimmed();

    if(idSegment.isEmpty())
    {
        qDebug() << "Aucun segment sélectionné pour suppression.";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM segment WHERE id_segment = :id_segment");
    query.bindValue(":id_segment", idSegment);

    if(!query.exec())
    {
        qDebug() << "Erreur lors de la suppression du segment:"
                 << query.lastError().text();
        return;
    }
    else
    {
        qDebug() << "Segment #" << idSegment << "supprimé avec succès!";
    }

    chargerSegmentsDepuisBDD();
}

void EditionSalle::modifierSegmentsBDD()
{
    QString segmentChoisi = menuSegments->currentText();

    QStringList partiesSegment = segmentChoisi.split(" - ");
    QString     idSegment  = partiesSegment.at(0).split("#").at(1).trimmed();
    QString     ipActuelle = partiesSegment.at(1).split(":").at(1).trimmed();

    if(idSegment.isEmpty())
    {
        qDebug() << "Aucun segment sélectionné pour modification.";
        return;
    }
    bool    ok;
    QString nouvelleIp = QInputDialog::getText(this,
                                               "Modifier l'adresse IP",
                                               "Nouvelle adresse IP:",
                                               QLineEdit::Normal,
                                               ipActuelle,
                                               &ok);

    if(!ok || nouvelleIp.isEmpty())
    {
        qDebug() << "Modification annulée ou adresse IP invalide.";
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE segment SET ip_segment = :ip_segment WHERE "
                  "id_segment = :id_segment");
    query.bindValue(":ip_segment", nouvelleIp);
    query.bindValue(":id_segment", idSegment);

    if(!query.exec())
    {
        qDebug() << "Erreur lors de la modification du segment:"
                 << query.lastError().text();
        return;
    }
    else
    {
        qDebug() << "Segment #" << idSegment << "modifié avec succès!";
    }

    chargerSegmentsDepuisBDD();
}

void EditionSalle::ajouterSegmentsBDD()
{
    int       id_salle;
    QString   ip_segment = ajoutIPSegment->text();
    QSqlQuery query;
    query.prepare("INSERT INTO segment (ip_segment, id_salle) VALUES "
                  "(:ip_segment, :id_salle)");
    query.bindValue(":id_salle", id_salle = 1);
    query.bindValue(":ip_segment", ip_segment);

    if(!query.exec())
    {
        qDebug() << "Erreur d'insertion :" << query.lastError().text();
        return;
    }
    else
    {
        qDebug() << "Segment ajouté avec succès!";
    }

    ajoutIPSegment->clear();

    chargerSegmentsDepuisBDD();
}

void EditionSalle::supprimerScenariosBDD()
{
    QString scenarioChoisi = menuScenarios->currentText();

    QStringList partieScenario = scenarioChoisi.split(" - ");
    QString     idScenario = partieScenario.at(0).split("#").at(1).trimmed();

    if(idScenario.isEmpty())
    {
        qDebug() << "Aucun scénario sélectionné pour suppression.";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM scenario WHERE id_scenario = :id_scenario");
    query.bindValue(":id_scenario", idScenario);

    if(!query.exec())
    {
        qDebug() << "Erreur lors de la suppression du scénario:"
                 << query.lastError().text();
        return;
    }
    else
    {
        qDebug() << "Scénario #" << idScenario << "supprimé avec succès!";
    }

    chargerScenariosDepuisBDD();
}

void EditionSalle::modifierScenariosBDD()
{
    QString scenarioChoisi = menuScenarios->currentText();

    QStringList partieScenario = scenarioChoisi.split(" - ");
    QString     idScenario = partieScenario.at(0).split("#").at(1).trimmed();
    QString     nomActuel  = partieScenario.at(1).trimmed();
    QString     intensiteActuelle =
      partieScenario.at(2).split("lux").at(0).trimmed();

    if(idScenario.isEmpty())
    {
        qDebug() << "Aucun scénario sélectionné pour modification.";
        return;
    }

    bool    ok;
    QString nouveauNom = QInputDialog::getText(this,
                                               "Modifier le nom du scénario",
                                               "Nouveau nom du scénario:",
                                               QLineEdit::Normal,
                                               nomActuel,
                                               &ok);
    QString nouvelleIntensite =
      QInputDialog::getText(this,
                            "Modifier l'intensité",
                            "Nouvelle intensité (en lux):",
                            QLineEdit::Normal,
                            intensiteActuelle,
                            &ok);

    if(!ok || nouveauNom.isEmpty() || nouvelleIntensite.isEmpty())
    {
        qDebug() << "Modification annulée ou valeurs invalides.";
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE scenario SET nom_scenario = :nom_scenario, "
                  "intensite_scenario = :intensite_scenario WHERE "
                  "id_scenario = :id_scenario");
    query.bindValue(":nom_scenario", nouveauNom);
    query.bindValue(":intensite_scenario", nouvelleIntensite);
    query.bindValue(":id_scenario", idScenario);

    if(!query.exec())
    {
        qDebug() << "Erreur lors de la modification du scénario:"
                 << query.lastError().text();
        return;
    }
    else
    {
        qDebug() << "Scénario #" << idScenario << "modifié avec succès!";
    }

    chargerScenariosDepuisBDD();
}

void EditionSalle::ajouterScenariosBDD()
{
    QString nomScenario       = ajoutScenario->text();
    QString intensiteScenario = ajoutIntensiteScenario->text();

    if(nomScenario.isEmpty() || intensiteScenario.isEmpty())
    {
        qDebug() << "Nom ou intensité du scénario manquant.";
        return;
    }

    QSqlQuery query;
    query.prepare(
      "INSERT INTO scenario (nom_scenario, intensite_scenario) VALUES "
      "(:nom_scenario, :intensite_scenario)");
    query.bindValue(":nom_scenario", nomScenario);
    query.bindValue(":intensite_scenario", intensiteScenario);

    if(!query.exec())
    {
        qDebug() << "Erreur d'insertion scénario :" << query.lastError().text();
        return;
    }
    else
    {
        qDebug() << "Scénario ajouté avec succès!";
    }

    ajoutScenario->clear();
    ajoutIntensiteScenario->clear();

    chargerScenariosDepuisBDD();
}
