#include "salleb22.h"
#include "elight.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

salleB22::salleB22(QWidget *parent) : QWidget(parent) {
  setWindowTitle("salleB22");

  QLabel *titreB22 = new QLabel("Salle B22", this);
  QLabel *labelLogoeLight = new QLabel;
  QLabel *conso = new QLabel("Consommation d'énergie:");
  QLabel *segments = new QLabel(this);
  QLabel *scenarios = new QLabel(this);
  QComboBox *menuScenario = new QComboBox(this);
  QComboBox *menuSegment = new QComboBox(this);
  QPushButton *retour = new QPushButton("Retour", this);

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreB22->setStyleSheet("font-weight: 900; font-size: 50px;");
  conso->setStyleSheet("border: 1px solid black; background-color: #FFFF33;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreB22, Qt::AlignHCenter);
  layout->addWidget(conso, 0, Qt::AlignHCenter);
  layout->addWidget(retour);
  layout->addWidget(segments, 0, Qt::AlignLeft);
  layout->addWidget(menuSegment);
  layout->addWidget(scenarios, Qt::AlignRight);
  layout->addWidget(menuScenario);

  segments->setText("Segments :");
  menuSegment->addItem("Segment 1");
  menuSegment->addItem("Segment 2");

  scenarios->setText("Scénarios :");
  menuScenario->addItem("Scénario 1");
  menuScenario->addItem("Scénario 2");

  connect(retour, &QPushButton::clicked, this, &salleB22::retourB22);
}

void salleB22::retourB22() { this->destroy(); }
