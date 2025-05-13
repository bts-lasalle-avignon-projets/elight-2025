#include "historique.h"
#include "communicationbasededonnees.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QPair>

Historique::Historique(QWidget* parent) :
    QWidget(parent), baseDeDonnees(CommunicationBaseDeDonnees::creerInstance())
{
    qDebug() << Q_FUNC_INFO << this;

    setWindowTitle(QString(TITRE_FENETRE_HISTORIQUE));

    titreHistorique =
      new QLabel("Historique de la consommation d'énergie", this);
    QLabel*      labelLogoeLight = new QLabel(this);
    QPixmap      logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    listeHistorique              = new QListWidget(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* entete = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);
    entete->addWidget(labelLogoeLight);
    entete->addWidget(titreHistorique, Qt::AlignBaseline);

    layout->addLayout(entete);
    layout->addWidget(listeHistorique);
    layout->addWidget(boutonFermeture);

    if(baseDeDonnees->connecter())
    {
        chargerHistoriqueDepuisBDD();
    }

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &Historique::fermerFenetre);

    this->setStyleSheet("background-color: #FFFFFF;");
    titreHistorique->setStyleSheet("font-weight: 900; font-size: 50px;");
#ifdef RASPBERRY_PI
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
#else
    setWindowFlags(Qt::Dialog);
#endif

    setWindowModality(Qt::WindowModal);
}

Historique::~Historique()
{
    CommunicationBaseDeDonnees::detruireInstance();
    qDebug() << Q_FUNC_INFO << this;
}

/**
 * @brief S'exécute à chaque fois que la fenêtre est affichée
 *
 * @fn Historique::showEvent
 *
 */
void Historique::showEvent(QShowEvent* event)
{
    Q_UNUSED(event)
    qDebug() << Q_FUNC_INFO << this;
}

void Historique::fermerFenetre()
{
    this->close();
}

void Historique::chargerHistoriqueDepuisBDD()
{
    qDebug() << Q_FUNC_INFO;
    QSqlQuery requete;
    requete.prepare(
      "SELECT id_segment, consommation, horodatage_releve FROM "
      "historique_consommation_segment ORDER BY horodatage_releve DESC");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    listeHistorique->clear();

    while(requete.next())
    {
        int     idSegment    = requete.value(ID_SEGMENT).toInt();
        float   consommation = requete.value(CONSOMMATION).toFloat();
        QString horodatage   = requete.value(HORODATAGE_RELEVE).toString();

        QString ligne = QString("Segment #%1 - %2 kWh - %3")
                          .arg(idSegment)
                          .arg(consommation, 0, 'f', 2)
                          .arg(horodatage);

        listeHistorique->addItem(ligne);
    }
}
