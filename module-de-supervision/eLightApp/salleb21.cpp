#include "salleb21.h"
#include "elight.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

salleB21::salleB21(QWidget *parent) : QWidget(parent) {
  setWindowTitle("salleB21");

  QLabel *titreB21 = new QLabel("Salle B21", this);
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
  titreB21->setStyleSheet("font-weight: 900; font-size: 50px;");
  conso->setStyleSheet("border: 1px solid black; background-color: #FFFF33;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreB21, Qt::AlignHCenter);
  layout->addWidget(conso, 0, Qt::AlignHCenter);
  layout->addWidget(retour);
  layout->addWidget(segments, 0, Qt::AlignLeft);
  layout->addWidget(menuSegment);
  layout->addWidget(scenarios, Qt::AlignRight);
  layout->addWidget(menuScenario);

  segments->setText("Segments :");
  menuSegment->addItem("PLACEHOLDER 1");
  menuSegment->addItem("PLACEHOLDER 2");

  scenarios->setText("Scénarios :");
  menuScenario->addItem("PLACEHOLDER 1");
  menuScenario->addItem("PLACEHOLDER 2");

  connect(retour, &QPushButton::clicked, this, &salleB21::retourB21);
}

void salleB21::retourB21() { this->destroy(); }
