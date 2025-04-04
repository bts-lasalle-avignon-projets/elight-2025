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
        chargerHistoriqueDepuisBDD();
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
    QSqlQuery requete;
    requete.prepare(
      "SELECT id_segment, consommation, horodatage_releve FROM "
      "historique_consommation_segment ORDER BY horodatage_releve DESC");

    if(!requete.exec())
    {
        qDebug() << Q_FUNC_INFO << "Erreur SQL" << requete.lastError().text();
        return;
    }

    table->setRowCount(0);

    while(requete.next())
    {
        int     idSegment    = requete.value(0).toInt();
        float   consommation = requete.value(1).toFloat();
        QString horodatage   = requete.value(2).toString();

        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row,
                       0,
                       new QTableWidgetItem(QString::number(idSegment)));
        table->setItem(
          row,
          1,
          new QTableWidgetItem(QString::number(consommation, 'f', 2)));
        table->setItem(row, 2, new QTableWidgetItem(horodatage));
    }
}
