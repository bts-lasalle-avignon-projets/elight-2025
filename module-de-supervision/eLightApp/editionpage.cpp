#include "editionpage.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

editionPage::editionPage(QWidget *parent) : QWidget(parent) {
  setWindowTitle("edition");

  QLabel *titreEdit = new QLabel("Édition", this);
  QLabel *labelLogoeLight = new QLabel;
  QPushButton *retour = new QPushButton("Retour", this);
  QLabel *segments = new QLabel(this);
  QLabel *scenarios = new QLabel(this);
  QPushButton *supprimer = new QPushButton(this);
  QPushButton *modif = new QPushButton(this);
  QPushButton *ajout = new QPushButton(this);
  QComboBox *menuScenario = new QComboBox(this);
  QComboBox *menuSegment = new QComboBox(this);

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreEdit->setStyleSheet("font-weight: 900; font-size: 90px;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;
  QGridLayout *button = new QGridLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);
  layout->addWidget(retour);
  layout->addWidget(segments, 0, Qt::AlignLeft);
  layout->addWidget(menuSegment);
  layout->addLayout(button);
  layout->addWidget(scenarios, Qt::AlignRight);
  layout->addWidget(menuScenario);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreEdit, Qt::AlignBaseline);

  button->addWidget(ajout, 0, 1);
  button->addWidget(modif, 0, 2);
  button->addWidget(supprimer, 0, 3);

  supprimer->setText("Supprimer");
  modif->setText("Modifier");
  ajout->setText("Ajouter");

  segments->setText("Segments :");
  menuSegment->addItem("Segment 1");
  menuSegment->addItem("Segment 2");

  scenarios->setText("Scénarios :");
  menuScenario->addItem("Scénario 1");
  menuScenario->addItem("Scénario 2");

  connect(retour, &QPushButton::clicked, this, &editionPage::retourEdit);
}

void editionPage::retourEdit() { this->destroy(); }
