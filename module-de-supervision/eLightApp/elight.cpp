#include "elight.h"
#include "ui_elight.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

eLight::eLight(QWidget *parent) : QWidget(parent), ui(new Ui::eLight) {
  ui->setupUi(this);
  setWindowTitle("eLight");

  QLabel *titre = new QLabel("eLight", this);
  QLabel *labelLogoeLight = new QLabel;
  QLabel *consoT = new QLabel("Consommation total:", this);

  QPushButton *b20 = new QPushButton("Salle B20 \n Consommation:", this);
  QPushButton *b21 = new QPushButton("Salle B21 \n Consommation:", this);
  QPushButton *b22 = new QPushButton("Salle B22 \n Consommation:", this);
  QPushButton *ajout = new QPushButton("Ajouter une salle", this);
  QPushButton *historique = new QPushButton("Historique", this);

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titre->setStyleSheet("font-weight: 900; font-size: 90px;");
  b20->setStyleSheet("QPushButton{ background-color: #70eb65; }"); // active
  b21->setStyleSheet("QPushButton{ background-color: #eb6565; }"); // inactive
  b22->setStyleSheet("QPushButton{ background-color: #eb6565; }"); // inactive
  consoT->setStyleSheet("border: 1px solid black; background-color: #FFFF33;");

  titre->setObjectName("titreAccueil");
  consoT->setObjectName("consoTotal");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QGridLayout *salle = new QGridLayout;
  QHBoxLayout *logotitre = new QHBoxLayout;
  QHBoxLayout *piedPage = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);
  piedPage->setAlignment(Qt::AlignCenter);

  layout->addLayout(logotitre);
  layout->addLayout(salle);
  layout->addLayout(piedPage);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titre, Qt::AlignBaseline);

  salle->addWidget(b20, 0, 0);
  salle->addWidget(b21, 0, 1);
  salle->addWidget(b22, 0, 2);
  salle->addWidget(ajout, 1, 0);

  piedPage->addWidget(historique);
  piedPage->addWidget(consoT);

  chargeurFeuilleStyle();
}

eLight::~eLight() { delete ui; }

void eLight::chargeurFeuilleStyle() {
  QString cheminRessource = "./" + QString(CHEMIN_RESSOURCE) + "/";
  QFile styleFile(cheminRessource + QString(STYLE_APPLICATION));
  if (styleFile.open(QFile::ReadOnly)) {
    QTextStream in(&styleFile);
    QString style = in.readAll();
    this->setStyleSheet(style);
  }
}
