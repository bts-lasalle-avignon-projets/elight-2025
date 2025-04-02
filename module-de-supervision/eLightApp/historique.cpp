#include "historique.h"
#include "communicationbasededonnees.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QPair>

Historique::Historique(QWidget* parent) :
    QWidget(parent), baseDeDonnees(new CommunicationBaseDeDonnees(this))
{
    qDebug() << Q_FUNC_INFO << this;

    setWindowTitle(QString(TITRE_FENETRE_HISTORIQUE));

    titreHistorique =
      new QLabel("Historique de la consommation d'énergie", this);
    QLabel*      labelLogoeLight = new QLabel(this);
    QPixmap      logoeLight(QString(CHEMIN_RESSOURCE) + "logo-elight.png");
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);
    table                        = new QTableWidget(this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    QHBoxLayout* entete = new QHBoxLayout;

    labelLogoeLight->setPixmap(logoeLight);
    entete->addWidget(labelLogoeLight);
    entete->addWidget(titreHistorique, Qt::AlignBaseline);

    layout->addLayout(entete);
    layout->addWidget(table);
    layout->addWidget(boutonFermeture);

    if(baseDeDonnees->connecter())
    {
        qDebug() << "Connexion réussi";
        chargerHistoriqueDepuisBDD();
    }
    else
    {
        qDebug() << "Connexion échoué";
    }

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &Historique::fermerFenetre);

    this->setStyleSheet("background-color: #FFFFFF;");
    titreHistorique->setStyleSheet("font-weight: 900; font-size: 50px;");
#ifdef RASPBERRY_PI
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Dialog); // Ajouter Qt::WindowStaysOnTopHint*/
#else
    setWindowFlags(Qt::Dialog);
#endif

    setWindowModality(Qt::WindowModal);

    initialiserTable();
}

Historique::~Historique()
{
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
    qDebug() << Q_FUNC_INFO << this;
}

void Historique::initialiserTable()
{
    QStringList nomColonnesTable;
    nomColonnesTable << "Salle"
                     << "Consommation (kWh)"
                     << "Horodatage";
    table->setColumnCount(nomColonnesTable.count());
    table->setHorizontalHeaderLabels(nomColonnesTable);

#ifdef DEMO_HISTORIQUE
    table->setRowCount(1);

    /*table->setItem(0, 0, new QTableWidgetItem("B20"));
    table->setItem(0, 1, new QTableWidgetItem("150 kWh"));
    table->setItem(0,
                   2,
                   new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

    table->setItem(1, 0, new QTableWidgetItem("B21"));
    table->setItem(1, 1, new QTableWidgetItem("100 kWh"));
    table->setItem(1,
                   2,
                   new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

    table->setItem(2, 0, new QTableWidgetItem("B22"));
    table->setItem(2, 1, new QTableWidgetItem("120 kWh"));
    table->setItem(2,
                   2,
                   new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

    table->setItem(3, 0, new QTableWidgetItem("B23"));
    table->setItem(3, 1, new QTableWidgetItem("130 kWh"));
    table->setItem(3,
                   2,
                   new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

    table->setItem(4, 0, new QTableWidgetItem("B24"));
    table->setItem(4, 1, new QTableWidgetItem("110 kWh"));
    table->setItem(4,
                   2,
                   new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy
    HH:mm")));*/
#endif

    QHeaderView* headerView = table->horizontalHeader();
    headerView->setSectionResizeMode(QHeaderView::Stretch);

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
}

void Historique::fermerFenetre()
{
    this->close();
}

void Historique::chargerHistoriqueDepuisBDD()
{
    int nombreLigne = table->rowCount();

    QSqlQuery requete;
    requete.prepare("SELECT id_segment, consommation, horodatage_releve FROM "
                    "historique_consommation_segment");

    if(!requete.exec())
    {
        qDebug() << "Erreur lors de la récupération de l'historique:"
                 << requete.lastError().text();
        return;
    }

    while(requete.next())
    {
        nombreLigne++;
        table->setRowCount(nombreLigne);

        int     idSegment    = requete.value(0).toInt();
        float   consommation = requete.value(1).toFloat();
        QString horodatage   = requete.value(2).toString();

        table->setItem(nombreLigne - 1,
                       0,
                       new QTableWidgetItem(QString::number(idSegment)));
        table->setItem(nombreLigne - 1,
                       1,
                       new QTableWidgetItem(QString::number(consommation)));
        table->setItem(nombreLigne - 1, 2, new QTableWidgetItem(horodatage));
    }
}
