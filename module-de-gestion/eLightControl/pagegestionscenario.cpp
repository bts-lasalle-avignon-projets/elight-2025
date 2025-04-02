#include "pagegestionscenario.h"

PageGestionScenario::PageGestionScenario(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    QLabel* titreCreationScenario = new QLabel(this);
    boutonRetourGestionScenario   = new QPushButton(this);

    QLabel*      texteEnregistrerScenario      = new QLabel(this);
    QLabel*      nomEnregistrerScenario        = new QLabel(this);
    QLineEdit*   boiteNomScenario              = new QLineEdit(this);
    QLabel*      intensiteEnregistrerScenario  = new QLabel(this);
    QLineEdit*   boiteIntensiteScenario        = new QLineEdit(this);
    QPushButton* boutonValiderCreationScenario = new QPushButton(this);

    QFrame* separateurGestionScenarios = new QFrame(this);

    QLabel* titreModificationSuppressionScenario = new QLabel(this);

    QLabel*      texteListeScenarios     = new QLabel(this);
    QComboBox*   listeScenarios          = new QComboBox(this);
    QPushButton* boutonSupprimerScenario = new QPushButton(this);

    QLabel*      texteModifierScenarioExistant     = new QLabel(this);
    QLabel*      nouveauNomModifierScenario        = new QLabel(this);
    QLineEdit*   boiteNouveauNom                   = new QLineEdit(this);
    QLabel*      nouvelleIntensiteModifierScenario = new QLabel(this);
    QLineEdit*   boiteNouvelleIntensite            = new QLineEdit(this);
    QPushButton* boutonValiderModifierScenario     = new QPushButton(this);

    titreCreationScenario->setText("<h1>Création</h1>");
    boutonRetourGestionScenario->setText("RETOUR");

    texteEnregistrerScenario->setText("Enregistrer un scénario : ");
    nomEnregistrerScenario->setText("NOM");
    boiteNomScenario->setPlaceholderText("...");
    intensiteEnregistrerScenario->setText("INTENSITE");
    boiteIntensiteScenario->setPlaceholderText("...");
    boutonValiderCreationScenario->setText("CONFIRMER");

    separateurGestionScenarios->setFrameShape(QFrame::HLine);
    separateurGestionScenarios->setFrameShadow(QFrame::Sunken);
    separateurGestionScenarios->setObjectName("separateurGestionScenarios");
    separateurGestionScenarios->setFixedHeight(10);

    titreModificationSuppressionScenario->setText(
      "<h1>Modification & Suppression</h1>");

    texteListeScenarios->setText("Liste des scénarios : ");
    listeScenarios->addItem("PLACEHOLDER 1");
    listeScenarios->addItem("PLACEHOLDER 2");
    boutonSupprimerScenario->setText("Supprimer");

    texteModifierScenarioExistant->setText("Modifier un scénario existant : ");
    nouveauNomModifierScenario->setText("Nouveau nom");
    boiteNouveauNom->setPlaceholderText("...");
    nouvelleIntensiteModifierScenario->setText("Nouvelle intensité lumineuse");
    boiteNouvelleIntensite->setPlaceholderText("...");
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

    if(baseDeDonnees.connecter())
    {
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
