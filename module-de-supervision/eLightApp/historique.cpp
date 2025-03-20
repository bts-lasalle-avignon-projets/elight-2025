#include "historique.h"
#include "elight.h"

#include <QDateTime>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QTableWidget>
#include <QtWidgets>

Historique::Historique(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Historique");

  QLabel *titreHist =
      new QLabel("Historique de la consommation d'énergie", this);
  QLabel *labelLogoeLight = new QLabel;

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreHist->setStyleSheet("font-weight: 900; font-size: 50px;");

  QPushButton *retour = new QPushButton("Retour", this);

  QTableWidget *table = new QTableWidget(this);
  table->setColumnCount(3);
  table->setHorizontalHeaderLabels(QStringList() << "Salle"
                                                 << "Consommation (kWh)"
                                                 << "Date/Heure");

  table->setRowCount(5);

  table->setItem(0, 0, new QTableWidgetItem("B20"));
  table->setItem(0, 1, new QTableWidgetItem("150 kWh"));
  table->setItem(0, 2,
                 new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

  table->setItem(1, 0, new QTableWidgetItem("B21"));
  table->setItem(1, 1, new QTableWidgetItem("100 kWh"));
  table->setItem(1, 2,
                 new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

  table->setItem(2, 0, new QTableWidgetItem("B22"));
  table->setItem(2, 1, new QTableWidgetItem("120 kWh"));
  table->setItem(2, 2,
                 new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

  table->setItem(3, 0, new QTableWidgetItem("B23"));
  table->setItem(3, 1, new QTableWidgetItem("130 kWh"));
  table->setItem(3, 2,
                 new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

  table->setItem(4, 0, new QTableWidgetItem("B24"));
  table->setItem(4, 1, new QTableWidgetItem("110 kWh"));
  table->setItem(4, 2,
                 new QTableWidgetItem(
                     QDateTime::currentDateTime().toString("dd/MM/yy HH:mm")));

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);
  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreHist, Qt::AlignBaseline);

  layout->addLayout(logotitre);
  layout->addWidget(retour);
  layout->addWidget(table);

  table->resizeColumnsToContents();
  table->resizeRowsToContents();

  connect(retour, &QPushButton::clicked, this, &Historique::retourHistorique);
}

void Historique::retourHistorique() { this->destroy(); }
