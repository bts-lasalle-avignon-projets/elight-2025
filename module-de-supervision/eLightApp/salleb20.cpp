#include "salleb20.h"
#include "editionpage.h"
#include "elight.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

salleB20::salleB20(QWidget *parent) : QWidget(parent) {
  setWindowTitle("salleB20");

  QLabel *titreB20 = new QLabel("Salle B20", this);
  QLabel *labelLogoeLight = new QLabel;
  QLabel *conso = new QLabel("Consommation d'énergie:");
  QLabel *segments = new QLabel(this);
  QLabel *scenarios = new QLabel(this);
  QComboBox *menuScenario = new QComboBox(this);
  QComboBox *menuSegment = new QComboBox(this);
  QPushButton *retour = new QPushButton("Retour", this);
  QPushButton *editB20 = new QPushButton("Édition", this);

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreB20->setStyleSheet("font-weight: 900; font-size: 50px;");
  conso->setStyleSheet("border: 1px solid black; background-color: #FFFF33;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreB20, Qt::AlignCenter);
  layout->addWidget(conso, 0, Qt::AlignHCenter);
  layout->addWidget(retour);
  layout->addWidget(editB20);
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

  connect(retour, &QPushButton::clicked, this, &salleB20::retourB20);
  connect(editB20, &QPushButton::clicked, this, &salleB20::editionB20);
}

void salleB20::retourB20() { this->destroy(); }

void salleB20::editionB20() {
  editionPage *editPage = new editionPage();
  editPage->show();
}
