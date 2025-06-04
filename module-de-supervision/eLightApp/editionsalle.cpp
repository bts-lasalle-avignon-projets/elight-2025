#include "editionsalle.h"
#include "communicationbasededonnees.h"
#include "salle.h"
#include <QDebug>

EditionSalle::EditionSalle(Salle* salle, QWidget* parent) :
    QWidget(parent), salle(salle),
    baseDeDonnees(CommunicationBaseDeDonnees::creerInstance())
{
    setWindowTitle(QString(TITRE_FENETRE_EDITION));

    int idSalle = salle->getIdSalle();
    qDebug() << Q_FUNC_INFO << this << "idSalle" << idSalle;

    QPixmap      logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QLabel*      titreEdition    = new QLabel("Édition", this);
    QLabel*      labelLogoeLight = new QLabel(this);
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    QPushButton* boutonSauvegardeScenarios =
      new QPushButton("Sauvegarder scénarios", this);
    QPushButton* boutonSauvegardeSegments =
      new QPushButton("Sauvegarder segments", this);
    QLabel* segments             = new QLabel(this);
    QLabel* scenarios            = new QLabel(this);
    menuScenarios                = new QComboBox(this);
    menuSegments                 = new QComboBox(this);
    ajoutIPSegment               = new QLineEdit(this);
    ajoutScenario                = new QLineEdit(this);
    ajoutIntensiteScenario       = new QLineEdit(this);
    QPushButton* validerSegment  = new QPushButton("Ajouter", this);
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
    layout->addWidget(boutonSauvegardeSegments);
    layout->addWidget(scenarios, Qt::AlignRight);
    layout->addWidget(menuScenarios);
    layout->addLayout(editionScenario);
    layout->addWidget(boutonSauvegardeScenarios);
    layout->addWidget(boutonFermeture);

    segments->setText("Segments :");

    scenarios->setText("Scénarios :");

    if(baseDeDonnees->connecter())
    {
        chargerScenariosDepuisBDD();
        chargerSegmentsDepuisBDD();
    }

    emit scenarioModifie();

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &EditionSalle::fermerFenetre);

    connect(boutonSauvegardeScenarios,
            &QPushButton::clicked,
            this,
            &EditionSalle::sauvegarderFenetreScenarios);

    connect(boutonSauvegardeSegments,
            &QPushButton::clicked,
            this,
            &EditionSalle::sauvegarderFenetreSegments);

    connect(validerSegment,
            &QPushButton::clicked,
            this,
            &EditionSalle::ajouterSegmentsBDD);

    connect(supprSegment,
            &QPushButton::clicked,
            this,
            &EditionSalle::supprimerSegmentsBDD);

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

EditionSalle::~EditionSalle()
{
    CommunicationBaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO << this;
}

void EditionSalle::fermerFenetre()
{
    this->close();
}

void EditionSalle::sauvegarderFenetreScenarios()
{
    int idSalle = salle->getIdSalle();
    qDebug() << Q_FUNC_INFO << "idSalle" << idSalle;

    QString     scenarioChoisi  = menuScenarios->currentText();
    QStringList partiesScenario = scenarioChoisi.split(" - ");
    QString     idScenario = partiesScenario.at(0).split("#").at(1).trimmed();

    if(!idScenario.isEmpty())
    {
        QSqlQuery querySegments;
        querySegments.prepare(
          "SELECT id_segment FROM segment WHERE id_salle = :id_salle");
        querySegments.bindValue(":id_salle", idSalle);

        if(!querySegments.exec())
        {
            qDebug()
              << Q_FUNC_INFO
              << "Erreur SQL lors de la récupération des segments de la salle"
              << querySegments.lastError().text();
            return;
        }

        while(querySegments.next())
        {
            QString idSegment =
              querySegments.value(COLONNE_ID_SEGMENT).toString();

            QSqlQuery updateSegmentQuery;
            updateSegmentQuery.prepare(
              "UPDATE segment SET id_scenario = :id_scenario WHERE id_segment "
              "= :id_segment");
            updateSegmentQuery.bindValue(":id_scenario", idScenario);
            updateSegmentQuery.bindValue(":id_segment", idSegment);

            if(!updateSegmentQuery.exec())
            {
                qDebug() << Q_FUNC_INFO
                         << "Erreur SQL lors de la mise à jour du segment avec "
                            "le scénario"
                         << updateSegmentQuery.lastError().text();
            }
        }
    }
    chargerScenariosDepuisBDD();

    // Récupérer les lux du scénario sélectionné
    QString luxStr =
      partiesScenario.at(2).split(" ").at(0).trimmed(); // "intensité lux"
    bool ok;
    int  intensite = luxStr.toInt(&ok);
    if(!ok)
    {
        qDebug() << Q_FUNC_INFO << "Erreur conversion intensité lux";
        return;
    }

    // Récupérer les IP des segments de la salle
    QSqlQuery querySegments;
    querySegments.prepare(
      "SELECT ip_segment FROM segment WHERE id_salle = :id_salle");
    querySegments.bindValue(":id_salle", idSalle);

    if(!querySegments.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL"
                 << querySegments.lastError().text();
        return;
    }

    while(querySegments.next())
    {
        QString ipSegment = querySegments.value(0).toString();

        QString    trame   = "#I;" + QString::number(intensite) + "\r\n";
        QByteArray donnees = trame.toUtf8();

        QHostAddress adresse(ipSegment);
        quint16      port = 5000;

        QUdpSocket socket;
        qint64     bytes = socket.writeDatagram(donnees, adresse, port);
        if(bytes == -1)
        {
            qDebug() << Q_FUNC_INFO << "Erreur envoi vers" << ipSegment
                     << socket.errorString();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "Trame envoyée à" << ipSegment << "->"
                     << trame.trimmed();
        }
    }
}

void EditionSalle::sauvegarderFenetreSegments()
{
    int idSalle = salle->getIdSalle();

    QString     segmentChoisi  = menuSegments->currentText();
    QStringList partiesSegment = segmentChoisi.split(" - ");
    QString     idSegment = partiesSegment.at(0).split("#").at(1).trimmed();
    qDebug() << Q_FUNC_INFO << "idSalle" << idSalle << "idSegment" << idSegment;

    if(!idSegment.isEmpty())
    {
        QSqlQuery querySegments;
        querySegments.prepare(
          "SELECT id_segment FROM segment WHERE id_salle = :id_salle");
        querySegments.bindValue(":id_salle", idSalle);

        if(!querySegments.exec())
        {
            qDebug()
              << Q_FUNC_INFO
              << "Erreur SQL lors de la récupération des segments de la salle"
              << querySegments.lastError().text();
            return;
        }

        while(querySegments.next())
        {
            QSqlQuery updateSalleQuery;
            updateSalleQuery.prepare("UPDATE segment SET id_salle = :id_salle "
                                     "WHERE id_segment = :id_segment");
            updateSalleQuery.bindValue(":id_salle", idSalle);
            updateSalleQuery.bindValue(":id_segment", idSegment);

            if(!updateSalleQuery.exec())
            {
                qDebug()
                  << Q_FUNC_INFO
                  << "Erreur SQL lors de la mise à jour du id_salle du segment"
                  << updateSalleQuery.lastError().text();
            }
        }
    }
    chargerSegmentsDepuisBDD();
}

void EditionSalle::chargerScenariosDepuisBDD()
{
    qDebug() << Q_FUNC_INFO;
    QSqlQuery requete;
    requete.prepare(
      "SELECT id_scenario, nom_scenario, intensite_scenario FROM scenario");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    menuScenarios->clear();

    while(requete.next())
    {
        QString idScenario  = requete.value(COLONNE_ID_SCENARIO).toString();
        QString nomScenario = requete.value(COLONNE_NOM_SCENARIO).toString();
        QString intensiteScenario =
          requete.value(COLONNE_INTENSITE_SCENARIO).toString();

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
    qDebug() << Q_FUNC_INFO;
    QSqlQuery requete;
    requete.prepare("SELECT id_segment, ip_segment FROM segment");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    menuSegments->clear();

    while(requete.next())
    {
        QString idSegment = requete.value(COLONNE_ID_SEGMENT).toString();
        QString ipSegment = requete.value(COLONNE_IP_SEGMENT).toString();

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
    qDebug() << Q_FUNC_INFO << "idSegment" << idSegment;

    if(idSegment.isEmpty())
    {
        qDebug() << Q_FUNC_INFO << "Aucun segment sélectionné pour suppression";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM segment WHERE id_segment = :id_segment");
    query.bindValue(":id_segment", idSegment);

    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << query.lastError().text();
        return;
    }

    chargerSegmentsDepuisBDD();
}

void EditionSalle::ajouterSegmentsBDD()
{
    int     id_salle;
    QString ip_segment = ajoutIPSegment->text();

    qDebug() << Q_FUNC_INFO << "ip_segment" << ip_segment;

    QSqlQuery query;
    query.prepare("INSERT INTO segment (ip_segment, id_salle) VALUES "
                  "(:ip_segment, :id_salle)");
    query.bindValue(":id_salle", id_salle = 1);
    query.bindValue(":ip_segment", ip_segment);

    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << query.lastError().text();
        return;
    }

    ajoutIPSegment->clear();

    chargerSegmentsDepuisBDD();
}

void EditionSalle::supprimerScenariosBDD()
{
    QString scenarioChoisi = menuScenarios->currentText();

    QStringList partieScenario = scenarioChoisi.split(" - ");
    QString     idScenario = partieScenario.at(0).split("#").at(1).trimmed();
    qDebug() << Q_FUNC_INFO << "idScenario" << idScenario;

    if(idScenario.isEmpty())
    {
        qDebug() << Q_FUNC_INFO
                 << "Aucun scénario sélectionné pour suppression";
        return;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM scenario WHERE id_scenario = :id_scenario");
    query.bindValue(":id_scenario", idScenario);

    if(!query.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << query.lastError().text();
        return;
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
    qDebug() << Q_FUNC_INFO << "idScenario" << idScenario;

    if(idScenario.isEmpty())
    {
        qDebug() << Q_FUNC_INFO
                 << "Aucun scénario sélectionné pour modification";
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
        qDebug() << Q_FUNC_INFO << "Modification annulée ou valeurs invalides";
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
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << query.lastError().text();
        return;
    }

    chargerScenariosDepuisBDD();
}

void EditionSalle::ajouterScenariosBDD()
{
    QString nomScenario       = ajoutScenario->text();
    QString intensiteScenario = ajoutIntensiteScenario->text();

    qDebug() << Q_FUNC_INFO << "nomScenario" << nomScenario
             << "intensiteScenario" << intensiteScenario;

    if(nomScenario.isEmpty() || intensiteScenario.isEmpty())
    {
        qDebug() << Q_FUNC_INFO << "Nom ou intensité du scénario manquant";
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
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << query.lastError().text();
        return;
    }

    ajoutScenario->clear();
    ajoutIntensiteScenario->clear();

    chargerScenariosDepuisBDD();
}
