#include "pagegestionscenario.h"
#include "config.h"
#include <QDebug>

PageGestionScenario::PageGestionScenario(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::getInstance())
{
    QString cheminRessources = "./" + QString(CHEMIN_RESSOURCES) + "/";
    QIcon   iconeValider(cheminRessources + QString(ICONE_VALIDER_GESTION));
    QIcon   iconeSupprimer(cheminRessources + QString(ICONE_SUPPRIMER_GESTION));

    QLabel* titreCreationScenario = new QLabel(this);

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

    QFont police;
    police.setPointSize(TAILLE_POLICE);

    titreCreationScenario->setFont(police);
    texteEnregistrerScenario->setFont(police);
    nomEnregistrerScenario->setFont(police);
    intensiteEnregistrerScenario->setFont(police);
    titreModificationSuppressionScenario->setFont(police);
    texteListeScenarios->setFont(police);
    listeScenarios->setFont(police);
    texteModifierScenarioExistant->setFont(police);
    nouveauNomModifierScenario->setFont(police);
    nouvelleIntensiteModifierScenario->setFont(police);
    boiteNouveauNom->setFont(police);
    boiteNouvelleIntensite->setFont(police);

    titreCreationScenario->setText("<h1>Création</h1><br>");

    texteEnregistrerScenario->setText("<b>Enregistrer un scénario : </b>");
    nomEnregistrerScenario->setText("Nom");
    nomEnregistrerScenario->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Preferred);
    nomEnregistrerScenario->setObjectName("nomEnregistrerScenario");
    boiteNomScenario->setPlaceholderText("...");
    boiteNomScenario->setSizePolicy(QSizePolicy::Expanding,
                                    QSizePolicy::Preferred);
    intensiteEnregistrerScenario->setText("Intensité lumineuse - 750 max");
    intensiteEnregistrerScenario->setSizePolicy(QSizePolicy::Expanding,
                                                QSizePolicy::Preferred);
    intensiteEnregistrerScenario->setObjectName("intensiteEnregistrerScenario");
    boiteIntensiteScenario->setPlaceholderText("...");
    boiteIntensiteScenario->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Preferred);
    boiteIntensiteScenario->setValidator(
      new QIntValidator(INTENSITE_MIN, INTENSITE_MAX, this));

    boutonValiderCreationScenario->setIcon(iconeValider);
    boutonValiderCreationScenario->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS_GESTION, HAUTEUR_ICONES_BOUTONS_GESTION));
    boutonValiderCreationScenario->setFixedSize(
      LARGEUR_ICONES_BOUTONS_GESTION + PADDING_ICONES_GESTION,
      HAUTEUR_ICONES_BOUTONS_GESTION + PADDING_ICONES_GESTION);
    boutonValiderCreationScenario->setObjectName(
      "boutonValiderCreationScenario");

    separateurGestionScenarios->setFrameShape(QFrame::HLine);
    separateurGestionScenarios->setFrameShadow(QFrame::Sunken);
    separateurGestionScenarios->setObjectName("separateurGestionScenarios");
    separateurGestionScenarios->setFixedHeight(10);

    titreModificationSuppressionScenario->setText(
      "<h1>Modification & Suppression</h1><br>");

    texteListeScenarios->setText("<b>Liste des scénarios : </b>");
    listeScenarios->setSizePolicy(QSizePolicy::Expanding,
                                  QSizePolicy::Preferred);
    boutonSupprimerScenario->setIcon(iconeSupprimer);
    boutonSupprimerScenario->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS_GESTION, HAUTEUR_ICONES_BOUTONS_GESTION));
    boutonSupprimerScenario->setFixedSize(
      LARGEUR_ICONES_BOUTONS_GESTION + PADDING_ICONES_GESTION,
      HAUTEUR_ICONES_BOUTONS_GESTION + PADDING_ICONES_GESTION);
    boutonSupprimerScenario->setObjectName("boutonSupprimerScenario");

    texteModifierScenarioExistant->setText("<b>Modifier un scénario : </b>");
    nouveauNomModifierScenario->setText("Nouveau nom");
    nouveauNomModifierScenario->setObjectName("nouveauNomModifierScenario");
    boiteNouveauNom->setPlaceholderText("...");
    boiteNouveauNom->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Preferred);
    nouvelleIntensiteModifierScenario->setText("Nouvelle intensité lumineuse");
    nouvelleIntensiteModifierScenario->setObjectName(
      "nouvelleIntensiteModifierScenario");
    boiteNouvelleIntensite->setPlaceholderText("...");
    boiteNouvelleIntensite->setSizePolicy(QSizePolicy::Expanding,
                                          QSizePolicy::Preferred);
    boiteNouvelleIntensite->setValidator(
      new QIntValidator(INTENSITE_MIN, INTENSITE_MAX, this));

    boutonValiderModifierScenario->setIcon(iconeValider);
    boutonValiderModifierScenario->setIconSize(
      QSize(LARGEUR_ICONES_BOUTONS_GESTION, HAUTEUR_ICONES_BOUTONS_GESTION));
    boutonValiderModifierScenario->setFixedSize(
      LARGEUR_ICONES_BOUTONS_GESTION + PADDING_ICONES_GESTION,
      HAUTEUR_ICONES_BOUTONS_GESTION + PADDING_ICONES_GESTION);
    boutonValiderModifierScenario->setObjectName(
      "boutonValiderModifierScenario");

    QVBoxLayout* layoutVerticalPageGestionScenario = new QVBoxLayout(this);
    QHBoxLayout* layoutEnteteCreationScenario      = new QHBoxLayout;
    QHBoxLayout* layoutCreationScenario            = new QHBoxLayout;
    QVBoxLayout* layoutCreationScenarioNom         = new QVBoxLayout;
    QVBoxLayout* layoutCreationScenarioIntensite   = new QVBoxLayout;

    QHBoxLayout* layoutEnteteModificationSuppressionScenario = new QHBoxLayout;

    QGridLayout* layoutGridModificationSuppressionScenario = new QGridLayout;

    QHBoxLayout* layoutModifierScenario = new QHBoxLayout;

    QVBoxLayout* layoutModifierScenarioNom       = new QVBoxLayout;
    QVBoxLayout* layoutModifierScenarioIntensite = new QVBoxLayout;

    layoutVerticalPageGestionScenario->addLayout(layoutEnteteCreationScenario);
    layoutVerticalPageGestionScenario->addLayout(layoutCreationScenario);
    layoutVerticalPageGestionScenario->addStretch();
    layoutVerticalPageGestionScenario->addWidget(separateurGestionScenarios);
    layoutVerticalPageGestionScenario->addLayout(
      layoutEnteteModificationSuppressionScenario);
    layoutVerticalPageGestionScenario->addLayout(
      layoutGridModificationSuppressionScenario);
    layoutVerticalPageGestionScenario->addStretch();

    layoutEnteteCreationScenario->addWidget(titreCreationScenario);

    layoutCreationScenario->addWidget(texteEnregistrerScenario);
    layoutCreationScenario->addLayout(layoutCreationScenarioNom);
    layoutCreationScenarioNom->addWidget(nomEnregistrerScenario);
    layoutCreationScenarioNom->addWidget(boiteNomScenario);
    layoutCreationScenarioIntensite->addWidget(intensiteEnregistrerScenario);
    layoutCreationScenarioIntensite->addWidget(boiteIntensiteScenario);
    layoutCreationScenario->addLayout(layoutCreationScenarioIntensite);
    layoutCreationScenario->addWidget(boutonValiderCreationScenario);

    layoutEnteteModificationSuppressionScenario->addWidget(
      titreModificationSuppressionScenario);

    layoutGridModificationSuppressionScenario->addWidget(texteListeScenarios,
                                                         0,
                                                         0);
    layoutGridModificationSuppressionScenario->addWidget(
      texteModifierScenarioExistant,
      1,
      0);
    layoutGridModificationSuppressionScenario->addWidget(listeScenarios, 0, 1);
    layoutGridModificationSuppressionScenario->addLayout(layoutModifierScenario,
                                                         1,
                                                         1);
    layoutGridModificationSuppressionScenario->addWidget(
      boutonSupprimerScenario,
      0,
      2);
    layoutGridModificationSuppressionScenario->addWidget(
      boutonValiderModifierScenario,
      1,
      2);

    layoutModifierScenario->addLayout(layoutModifierScenarioNom);
    layoutModifierScenario->addLayout(layoutModifierScenarioIntensite);

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
}

void PageGestionScenario::chargerScenariosDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_scenario, nom_scenario FROM scenario");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
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

    QString nomScenario       = boiteNomScenario->text();
    QString intensiteScenario = boiteIntensiteScenario->text();

    QSqlQuery requete;
    requete.prepare("INSERT INTO scenario(nom_scenario, intensite_scenario) "
                    "VALUES(:nom_scenario, :intensite_scenario)");
    requete.bindValue(":nom_scenario", nomScenario);
    requete.bindValue(":intensite_scenario", intensiteScenario);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "nomScenario" << nomScenario
                 << "intensiteScenario" << intensiteScenario;
        boiteNomScenario->clear();
        boiteIntensiteScenario->clear();
    }
    chargerScenariosDepuisBDD();
}

void PageGestionScenario::supprimerScenario()
{
    QString nomScenario = listeScenarios->currentText();

    QSqlQuery requete;
    requete.prepare("DELETE FROM scenario WHERE nom_scenario = :nom_scenario");
    requete.bindValue(":nom_scenario", nomScenario);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "nomScenario" << nomScenario;
    }
    chargerScenariosDepuisBDD();
}

void PageGestionScenario::modifierScenario()
{
    if(boiteNouvelleIntensite->text().toInt() > INTENSITE_MAX)
    {
        boiteNouvelleIntensite->setText(QString::number(INTENSITE_MAX));
    }

    QString nomScenario        = listeScenarios->currentText();
    QString nouveauNomScenario = boiteNouveauNom->text();
    QString nouvelleIntensite  = boiteNouvelleIntensite->text();

    QSqlQuery requete;

    requete.prepare("UPDATE scenario "
                    "SET nom_scenario = :nom_nouveau_scenario, "
                    "intensite_scenario = :nouvelle_intensite "
                    "WHERE nom_scenario = :nom_scenario");
    requete.bindValue(":nom_scenario", nomScenario);
    requete.bindValue(":nom_nouveau_scenario", nouveauNomScenario);
    requete.bindValue(":nouvelle_intensite", nouvelleIntensite);

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "nomScenario" << nomScenario
                 << "nouveauNomScenario" << nouveauNomScenario
                 << "nouvelleIntensite" << nouvelleIntensite;

        boiteNouveauNom->clear();
        boiteNouvelleIntensite->clear();
    }
    chargerScenariosDepuisBDD();
}
