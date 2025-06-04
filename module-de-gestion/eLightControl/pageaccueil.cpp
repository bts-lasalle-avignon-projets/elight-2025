#include "pageaccueil.h"
#include "config.h"
#include "boitesegment.h"
#include <QDebug>

PageAccueil::PageAccueil(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    qDebug() << Q_FUNC_INFO << this;

    QString cheminRessources = "./" + QString(CHEMIN_RESSOURCES) + "/";
    QIcon   iconeValider(cheminRessources + QString(ICONE_VALIDER_ACCUEIL));
    QIcon   iconeRetirer(cheminRessources + QString(ICONE_RETIRER_ACCUEIL));

    QFrame* separateurPageAccueil = new QFrame(this);

    QLabel* titreScenarioActif = new QLabel(this);

    QLabel* texteScenarioActif = new QLabel(this);
    nomScenarioActif           = new QLabel(this);
    intensiteScenarioActif     = new QLabel(this);
    boutonRetirerScenarioActif = new QPushButton(this);

    QLabel* texteSelectionScenario   = new QLabel(this);
    menuDeroulantScenarios           = new QComboBox(this);
    boutonConfirmerSelectionScenario = new QPushButton(this);

    QLabel* titreSegments = new QLabel(this);

    QFont police;
    police.setPointSize(TAILLE_POLICE);

    titreScenarioActif->setFont(police);
    texteScenarioActif->setFont(police);
    nomScenarioActif->setFont(police);
    intensiteScenarioActif->setFont(police);
    texteSelectionScenario->setFont(police);
    menuDeroulantScenarios->setFont(police);

    boutonRetirerScenarioActif->setIcon(iconeRetirer);
    boutonRetirerScenarioActif->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS_ACCUEIL, HAUTEUR_ICONES_BOUTONS_ACCUEIL));
    boutonRetirerScenarioActif->setFixedSize(
      LARGEUR_ICONES_BOUTONS_ACCUEIL + PADDING_ICONES_ACCUEIL,
      HAUTEUR_ICONES_BOUTONS_ACCUEIL + PADDING_ICONES_ACCUEIL);

    boutonConfirmerSelectionScenario->setIcon(iconeValider);
    boutonConfirmerSelectionScenario->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS_ACCUEIL, HAUTEUR_ICONES_BOUTONS_ACCUEIL));
    boutonConfirmerSelectionScenario->setFixedSize(
      LARGEUR_ICONES_BOUTONS_ACCUEIL + PADDING_ICONES_ACCUEIL,
      HAUTEUR_ICONES_BOUTONS_ACCUEIL + PADDING_ICONES_ACCUEIL);
    boutonConfirmerSelectionScenario->setObjectName(
      "boutonConfirmerSelectionScenario");

    titreSegments->setFont(police);

    separateurPageAccueil->setFrameShape(QFrame::HLine);
    separateurPageAccueil->setFrameShadow(QFrame::Sunken);
    separateurPageAccueil->setObjectName("separateurPageAccueil");
    separateurPageAccueil->setFixedHeight(10);

    titreScenarioActif->setText("<h1>Scénario</h1><br>");

    texteScenarioActif->setText("<b>Scénario actif : </b>");
    nomScenarioActif->setObjectName("nomScenarioActif");
    intensiteScenarioActif->setObjectName("intensiteScenarioActif");

    boutonRetirerScenarioActif->setObjectName("boutonRetirerScenarioActif");

    texteSelectionScenario->setText("<b>Sélection scénario : </b>");

    menuDeroulantScenarios->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Preferred);

    titreSegments->setText("<h1>Segments</h1><br>");

    QVBoxLayout* layoutVerticalPageAccueil            = new QVBoxLayout(this);
    QHBoxLayout* layoutEnteteScenarioActif            = new QHBoxLayout;
    QHBoxLayout* layoutHorizontalScenario             = new QHBoxLayout;
    QVBoxLayout* layoutScenarioActifSelectionScenario = new QVBoxLayout;
    QVBoxLayout* layoutNomIntensiteListeScenarios     = new QVBoxLayout;
    QHBoxLayout* layoutScenarioActif                  = new QHBoxLayout;
    QHBoxLayout* layoutSelectionScenario              = new QHBoxLayout;
    QHBoxLayout* layoutEnteteSegments                 = new QHBoxLayout;
    layoutSegments                                    = new QGridLayout;

    layoutVerticalPageAccueil->addLayout(layoutEnteteScenarioActif);
    layoutVerticalPageAccueil->addLayout(layoutHorizontalScenario);
    layoutVerticalPageAccueil->addStretch();
    layoutVerticalPageAccueil->addWidget(separateurPageAccueil);
    layoutVerticalPageAccueil->addLayout(layoutEnteteSegments);
    layoutVerticalPageAccueil->addLayout(layoutSegments);
    layoutVerticalPageAccueil->addStretch();

    layoutEnteteScenarioActif->addWidget(titreScenarioActif);

    layoutHorizontalScenario->addLayout(layoutScenarioActifSelectionScenario);
    layoutHorizontalScenario->addLayout(layoutNomIntensiteListeScenarios);

    layoutScenarioActifSelectionScenario->addWidget(texteScenarioActif);
    layoutScenarioActifSelectionScenario->addWidget(texteSelectionScenario);

    layoutNomIntensiteListeScenarios->addLayout(layoutScenarioActif);
    layoutNomIntensiteListeScenarios->addLayout(layoutSelectionScenario);

    layoutScenarioActif->addWidget(nomScenarioActif);
    layoutScenarioActif->addWidget(intensiteScenarioActif);
    layoutScenarioActif->addWidget(boutonRetirerScenarioActif);

    layoutSelectionScenario->addWidget(menuDeroulantScenarios);
    layoutSelectionScenario->addWidget(boutonConfirmerSelectionScenario);

    layoutEnteteSegments->addWidget(titreSegments);

    communicationSegments = new CommunicationSegments(this);

    connect(communicationSegments,
            &CommunicationSegments::puissanceInstantaneeSegmentRecue,
            this,
            [=](int idSegment, float nouvellePuissance) {
                for(BoiteSegment* segment: listeSegments)
                {
                    if(segment->getIdSegment() == idSegment)
                    {
                        segment->setPuissance(nouvellePuissance);
                        break;
                    }
                }
            });

    if(baseDeDonnees.connecter())
    {
        chargerScenariosDepuisBDD();
        chargerScenarioActifDepuisBDD();
        chargerSegmentsDepuisBDD();
    }

    connect(boutonRetirerScenarioActif, &QPushButton::clicked, this, [=] {
        retirerScenarioActif();
        for(BoiteSegment* segment: listeSegments)
        {
            emit signalEnvoyerTrameIntensite(segment->getIdSegment(),
                                             intensiteScenarioActifEntier);
            break;
        }
    });

    connect(boutonConfirmerSelectionScenario, &QPushButton::clicked, this, [=] {
        selectionnerScenarioActif();
        for(BoiteSegment* segment: listeSegments)
        {
            emit signalEnvoyerTrameIntensite(segment->getIdSegment(),
                                             intensiteScenarioActifEntier);
            break;
        }
    });
}

PageAccueil::~PageAccueil()
{
    qDebug() << Q_FUNC_INFO << this;
}

void PageAccueil::chargerScenariosDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_scenario, nom_scenario FROM scenario");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
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

void PageAccueil::chargerScenarioActifDepuisBDD()
{
    QString nomSalle;

    if(recupererNomSalle(nomSalle))
    {
        QSqlQuery requete;
        requete.prepare(
          "SELECT segment.id_scenario, scenario.nom_scenario, "
          "scenario.intensite_scenario FROM segment JOIN "
          "salle ON segment.id_salle = salle.id_salle LEFT JOIN scenario ON "
          "segment.id_scenario = scenario.id_scenario WHERE salle.nom_salle = "
          ":nom_salle LIMIT 1");

        requete.bindValue(":nom_salle", nomSalle);

        if(!requete.exec())
        {
            qDebug() << Q_FUNC_INFO << "Erreur SQL"
                     << requete.lastError().text();
            return;
        }

        if(requete.next())
        {
            QVariant idScenario        = requete.value(0);
            QString  nomScenario       = requete.value(1).toString();
            QString  intensiteScenario = requete.value(2).toString();

            if(idScenario.isNull())
            {
                nomScenarioActif->setText("Aucun scénario actif");
                intensiteScenarioActif->setText("");
                intensiteScenarioActifEntier = 0;
            }
            else
            {
                nomScenarioActif->setText(nomScenario);
                intensiteScenarioActif->setText(intensiteScenario + " lux");
                intensiteScenarioActifEntier = intensiteScenario.toInt();
            }
        }
        else
        {
            nomScenarioActif->setText("Aucune donnée trouvée");
            intensiteScenarioActif->setText("");
            intensiteScenarioActifEntier = 0;
        }
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Erreur récupération nom de la salle";
    }
}

void PageAccueil::selectionnerScenarioActif()
{
    QString nouveauScenarioActif = menuDeroulantScenarios->currentText();

    for(int i = 0; i < idsSegmentsSalle.size(); ++i)
    {
        QSqlQuery requete;
        requete.prepare(
          "UPDATE segment SET id_scenario = (SELECT id_scenario FROM scenario "
          "WHERE nom_scenario = :nom_scenario) WHERE id_segment = :id_segment");
        requete.bindValue(":nom_scenario", nouveauScenarioActif);
        requete.bindValue(":id_segment", idsSegmentsSalle[i]);

        if(!requete.exec())
        {
            qDebug() << Q_FUNC_INFO << "Erreur SQL"
                     << requete.lastError().text();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "nouveauScenarioActif"
                     << nouveauScenarioActif << "idSegment"
                     << idsSegmentsSalle[i];
        }
    }
    chargerScenariosDepuisBDD();
    chargerScenarioActifDepuisBDD();
}

void PageAccueil::retirerScenarioActif()
{
    for(int i = 0; i < idsSegmentsSalle.size(); ++i)
    {
        QSqlQuery requete;
        requete.prepare("UPDATE segment SET id_scenario = NULL WHERE "
                        "id_segment = :id_segment");
        requete.bindValue(":id_segment", idsSegmentsSalle[i]);

        if(!requete.exec())
        {
            qDebug() << Q_FUNC_INFO << "Erreur SQL"
                     << requete.lastError().text();
        }
        else
        {
            qDebug() << Q_FUNC_INFO << "idSegment" << idsSegmentsSalle[i];
        }
    }
    chargerScenariosDepuisBDD();
    chargerScenarioActifDepuisBDD();
}

void PageAccueil::chargerSegmentsDepuisBDD()
{
    QString nomSalle;
    bool    resultats = false;

    if(recupererNomSalle(nomSalle))
    {
        idsSegmentsSalle.clear();

        QSqlQuery requete;

        requete.prepare("SELECT segment.id_segment FROM segment "
                        "JOIN salle ON segment.id_salle = salle.id_salle "
                        "WHERE salle.nom_salle = :nom_salle");

        requete.bindValue(":nom_salle", nomSalle);

        if(!requete.exec())
        {
            qDebug() << Q_FUNC_INFO << "Erreur SQL"
                     << requete.lastError().text();
            return;
        }

        while(QLayoutItem* item = layoutSegments->takeAt(0))
        {
            if(QWidget* widget = item->widget())
            {
                widget->deleteLater();
            }
            delete item;
        }

        listeSegments.clear();

        int ligne   = 0;
        int colonne = 0;

        while(requete.next())
        {
            resultats = true;

            int idSegment = requete.value(0).toInt();
            idsSegmentsSalle.append(idSegment);

            QFont police;
            police.setPointSize(TAILLE_POLICE);

            QLabel* labelSegmentId =
              new QLabel(QString("<h2>Segment %1</h2>").arg(idSegment));
            labelSegmentId->setFont(police);
            labelSegmentId->setAlignment(Qt::AlignCenter);

            BoiteSegment* segment = new BoiteSegment(idSegment, this);
            listeSegments.append(segment);

            layoutSegments->addWidget(labelSegmentId, ligne, colonne);
            layoutSegments->addWidget(segment, ligne + 1, colonne);
            segment->setPuissance(0);

            connect(segment,
                    &BoiteSegment::segmentClique,
                    this,
                    [=](int idSegment) {
                        qDebug() << Q_FUNC_INFO << idSegment;
                        communicationSegments->envoyerTrameDemandePuissance(
                          communicationSegments->recupererAdresseDestination(
                            idSegment));
                    });

            connect(this, &PageAccueil::signalEnvoyerTrameIntensite, this, [=] {
                communicationSegments->envoyerTrameIntensite(
                  communicationSegments->recupererAdresseDestination(idSegment),
                  intensiteScenarioActifEntier);
            });

            communicationSegments->envoyerTrameIntensite(
              communicationSegments->recupererAdresseDestination(idSegment),
              intensiteScenarioActifEntier);

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
    else
    {
        qDebug() << Q_FUNC_INFO << "Erreur récupération nom de la salle";
    }
}

bool PageAccueil::recupererNomSalle(QString& nomSalle)
{
    QString cheminConfiguration = QCoreApplication::applicationDirPath() +
                                  QString("/") + QString(FICHIER_CONFIGURATION);

    if(QFile::exists(cheminConfiguration))
    {
        qDebug() << Q_FUNC_INFO << "cheminConfiguration" << cheminConfiguration;

        QSettings parametres(cheminConfiguration, QSettings::IniFormat);
        nomSalle = parametres.value("Salle/nom").toString();

        return true;
    }
    else
    {
        return false;
    }
}
