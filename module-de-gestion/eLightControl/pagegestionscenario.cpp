#include "pagegestionscenario.h"

PageGestionScenario::PageGestionScenario(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    QLabel* titreCreationScenario = new QLabel(this);
    boutonRetourGestionScenario   = new QPushButton(this);

    QLabel* texteEnregistrerScenario           = new QLabel(this);
    QLabel* nomEnregistrerScenario             = new QLabel(this);
    boiteNomScenario                           = new QLineEdit(this);
    QLabel* intensiteEnregistrerScenario       = new QLabel(this);
    boiteIntensiteScenario                     = new QLineEdit(this);
    QPushButton* boutonValiderCreationScenario = new QPushButton(this);

    QFrame* separateurGestionScenarios = new QFrame(this);

    QLabel* titreModificationSuppressionScenario = new QLabel(this);

    QLabel* texteListeScenarios          = new QLabel(this);
    listeScenarios                       = new QComboBox(this);
    QPushButton* boutonSupprimerScenario = new QPushButton(this);

    QLabel* texteModifierScenarioExistant      = new QLabel(this);
    QLabel* nouveauNomModifierScenario         = new QLabel(this);
    boiteNouveauNom                            = new QLineEdit(this);
    QLabel* nouvelleIntensiteModifierScenario  = new QLabel(this);
    boiteNouvelleIntensite                     = new QLineEdit(this);
    QPushButton* boutonValiderModifierScenario = new QPushButton(this);

    titreCreationScenario->setText("<h1>Création</h1>");
    boutonRetourGestionScenario->setText("RETOUR");

    texteEnregistrerScenario->setText("Enregistrer un scénario : ");
    nomEnregistrerScenario->setText("NOM");
    boiteNomScenario->setPlaceholderText("...");
    intensiteEnregistrerScenario->setText("INTENSITE");
    boiteIntensiteScenario->setPlaceholderText("...");
    boiteIntensiteScenario->setValidator(
      new QIntValidator(INTENSITE_MIN, INTENSITE_MAX, this));
    boutonValiderCreationScenario->setText("CONFIRMER");

    separateurGestionScenarios->setFrameShape(QFrame::HLine);
    separateurGestionScenarios->setFrameShadow(QFrame::Sunken);
    separateurGestionScenarios->setObjectName("separateurGestionScenarios");
    separateurGestionScenarios->setFixedHeight(10);

    titreModificationSuppressionScenario->setText(
      "<h1>Modification & Suppression</h1>");

    texteListeScenarios->setText("Liste des scénarios : ");
    boutonSupprimerScenario->setText("Supprimer");

    texteModifierScenarioExistant->setText("Modifier un scénario existant : ");
    nouveauNomModifierScenario->setText("Nouveau nom");
    boiteNouveauNom->setPlaceholderText("...");
    nouvelleIntensiteModifierScenario->setText("Nouvelle intensité lumineuse");
    boiteNouvelleIntensite->setPlaceholderText("...");
    boiteNouvelleIntensite->setValidator(
      new QIntValidator(INTENSITE_MIN, INTENSITE_MAX, this));
    boutonValiderModifierScenario->setText("Valider");

    QVBoxLayout* layoutVerticalPageGestionScenario = new QVBoxLayout(this);
    QHBoxLayout* layoutEnteteCreationScenario      = new QHBoxLayout;
    QHBoxLayout* layoutCreationScenario            = new QHBoxLayout;
    QVBoxLayout* layoutCreationScenarioNom         = new QVBoxLayout;
    QVBoxLayout* layoutCreationScenarioIntensite   = new QVBoxLayout;

    QHBoxLayout* layoutEnteteModificationCreationScenario = new QHBoxLayout;
    QHBoxLayout* layoutListeScenarios                     = new QHBoxLayout;
    QHBoxLayout* layoutModifierScenario                   = new QHBoxLayout;
    QVBoxLayout* layoutModifierScenarioNom                = new QVBoxLayout;
    QVBoxLayout* layoutModifierScenarioIntensite          = new QVBoxLayout;

    layoutVerticalPageGestionScenario->addLayout(layoutEnteteCreationScenario);
    layoutVerticalPageGestionScenario->addLayout(layoutCreationScenario);
    layoutVerticalPageGestionScenario->addStretch();
    layoutVerticalPageGestionScenario->addWidget(separateurGestionScenarios);
    layoutVerticalPageGestionScenario->addLayout(
      layoutEnteteModificationCreationScenario);
    layoutVerticalPageGestionScenario->addStretch();
    layoutVerticalPageGestionScenario->addLayout(layoutListeScenarios);
    layoutVerticalPageGestionScenario->addLayout(layoutModifierScenario);

    layoutEnteteCreationScenario->addWidget(titreCreationScenario);
    layoutEnteteCreationScenario->addWidget(boutonRetourGestionScenario);

    layoutCreationScenario->addWidget(texteEnregistrerScenario);
    layoutCreationScenario->addLayout(layoutCreationScenarioNom);
    layoutCreationScenarioNom->addWidget(nomEnregistrerScenario);
    layoutCreationScenarioNom->addWidget(boiteNomScenario);
    layoutCreationScenarioIntensite->addWidget(intensiteEnregistrerScenario);
    layoutCreationScenarioIntensite->addWidget(boiteIntensiteScenario);
    layoutCreationScenario->addLayout(layoutCreationScenarioIntensite);
    layoutCreationScenario->addWidget(boutonValiderCreationScenario);

    layoutEnteteModificationCreationScenario->addWidget(
      titreModificationSuppressionScenario);

    layoutListeScenarios->addWidget(texteListeScenarios);
    layoutListeScenarios->addWidget(listeScenarios);
    layoutListeScenarios->addWidget(boutonSupprimerScenario);

    layoutModifierScenario->addWidget(texteModifierScenarioExistant);
    layoutModifierScenario->addLayout(layoutModifierScenarioNom);
    layoutModifierScenario->addLayout(layoutModifierScenarioIntensite);
    layoutModifierScenario->addWidget(boutonValiderModifierScenario);

    layoutModifierScenarioNom->addWidget(nouveauNomModifierScenario);
    layoutModifierScenarioNom->addWidget(boiteNouveauNom);

    layoutModifierScenarioIntensite->addWidget(
      nouvelleIntensiteModifierScenario);
    layoutModifierScenarioIntensite->addWidget(boiteNouvelleIntensite);

    connect(boutonValiderCreationScenario, &QPushButton::clicked, this, [=] {
        enregistrerScenario();
    });
    connect(boutonSupprimerScenario, &QPushButton::clicked, this, [=] {
        supprimerScenario();
    });
    connect(boutonValiderModifierScenario, &QPushButton::clicked, this, [=] {
        modifierScenario();
    });

    if(baseDeDonnees.connecter())
    {
        chargerScenariosDepuisBDD();
    }
    else
    {
        qDebug() << "Erreur: Impossible de se connecter à la base de données";
    }
}

QPushButton* PageGestionScenario::getBoutonRetourGestionScenario() const
{
    return boutonRetourGestionScenario;
}

void PageGestionScenario::chargerScenariosDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_scenario, nom_scenario FROM scenario");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération des scénarios:"
                 << requete.lastError().text();
        return;
    }

    listeScenarios->clear();

    while(requete.next())
    {
        int     idScenario  = requete.value(0).toInt();
        QString nomScenario = requete.value(1).toString();

        listeScenarios->addItem(nomScenario, idScenario);
    }

    if(listeScenarios->count() == 0)
    {
        listeScenarios->addItem("Aucun scénario disponible");
    }
}

void PageGestionScenario::enregistrerScenario()
{
    if(boiteIntensiteScenario->text().toInt() > INTENSITE_MAX)
    {
        boiteIntensiteScenario->setText(QString::number(INTENSITE_MAX));
    }

    qDebug() << "Nom : " << boiteNomScenario->text()
             << " Intensite : " << boiteIntensiteScenario->text();

    /**
     * @todo ajouter la requête sql CREATE
     */
}

void PageGestionScenario::supprimerScenario()
{
    qDebug() << "Scénario selectionné : " << listeScenarios->currentText();

    /**
     * @todo ajouter la requête sql DELETE
     */
}
void PageGestionScenario::modifierScenario()
{
    if(boiteNouvelleIntensite->text().toInt() > INTENSITE_MAX)
    {
        boiteNouvelleIntensite->setText(QString::number(INTENSITE_MAX));
    }

    qDebug() << "Scénario selectionné : " << listeScenarios->currentText()
             << "Nom : " << boiteNouveauNom->text()
             << " Intensite : " << boiteNouvelleIntensite->text();

    /**
     * @todo ajouter la requête sql UPDATE
     */
}
