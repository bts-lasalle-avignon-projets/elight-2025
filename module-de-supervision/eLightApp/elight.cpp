/**
 * @file eLight.cpp
 *
 * @brief Définition de la classe ELight
 * @author Blondel Joshua
 * @version 1.0
 */

#include "elight.h"
#include "historique.h"
#include "salle.h"
#include <QDebug>

/**
 * @brief Constructeur de la classe ELight
 *
 * @fn ELight::ELight
 * @param parent L'adresse de l'objet parent, si nullptr ELight sera la
 * fenêtre principale de l'application
 */
ELight::ELight(QWidget* parent) :
    QWidget(parent), historiquePage(nullptr),
    baseDeDonnees(new CommunicationBaseDeDonnees(this))
{
    qDebug() << Q_FUNC_INFO << this;

    setWindowTitle(QString(APPLICATION) + QString(" v") + QString(VERSION));

#ifdef RASPBERRY_PI
    showFullScreen();
#endif

    QLabel* titre =
      new QLabel(QString(APPLICATION) + QString(" v") + QString(VERSION), this);
    QPixmap logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QLabel* labelLogoeLight    = new QLabel;
    QLabel* consommationTotale = new QLabel("Consommation totale : ", this);

    QPushButton* historique = new QPushButton("Historique", this);

    this->setStyleSheet("background-color: #FFFFFF;");
    titre->setStyleSheet("font-weight: 900; font-size: 90px;");
    consommationTotale->setStyleSheet(
      "border: 1px solid black; background-color: #FFFF33;");

    QVBoxLayout* layout   = new QVBoxLayout(this);
    QGridLayout* salle    = new QGridLayout;
    QHBoxLayout* entete   = new QHBoxLayout;
    QHBoxLayout* piedPage = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);
    piedPage->setAlignment(Qt::AlignCenter);

    layout->addLayout(entete);
    layout->addLayout(salle);
    layout->addLayout(piedPage);

    entete->addWidget(labelLogoeLight);
    entete->addWidget(titre, Qt::AlignBaseline);

    if(baseDeDonnees->connecter())
    {
        chargerSallesDepuisBDD();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "Connexion à la base de données échouée.";
    }

    for(int i = 0; i < boutonsSalles.count(); ++i)
    {
        salle->addWidget(boutonsSalles[i], 0, i);
    }

    piedPage->addWidget(historique);
    piedPage->addWidget(consommationTotale);

    connect(historique,
            &QPushButton::clicked,
            this,
            &ELight::afficherHistorique);
}

ELight::~ELight()
{
    qDebug() << Q_FUNC_INFO << this;
}

void ELight::afficherHistorique()
{
    if(historiquePage == nullptr)
        historiquePage = new Historique(this);
    historiquePage->show();
}

void ELight::afficherSalle()
{
    QPushButton* boutonSalle = qobject_cast<QPushButton*>(sender());

    if(salles[boutonSalle] == nullptr)
        salles[boutonSalle] = new Salle(boutonSalle->text(), this);

    salles[boutonSalle]->show();
}

void ELight::chargerSallesDepuisBDD()
{
    QSqlQuery requete;
    requete.prepare("SELECT id_salle, nom_salle FROM salle");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    int index = 0;
    while(requete.next())
    {
        QString nomSalle = requete.value(1).toString();
        int     idSalle  = requete.value(0).toInt();

        QPushButton* boutonSalle = new QPushButton(nomSalle, this);
        boutonSalle->setStyleSheet(
          index == 0 ? "QPushButton{ background-color: #70eb65; }"
                     : "QPushButton{ background-color: #eb6565; }");

        boutonsSalles.push_back(boutonSalle);
        salles[boutonSalle] = nullptr;

        connect(boutonSalle,
                &QPushButton::clicked,
                this,
                &ELight::afficherSalle);

        ++index;
    }

    if(index == 0)
    {
        qDebug() << Q_FUNC_INFO
                 << "Aucune salle trouvée dans la base de données.";
    }
}
