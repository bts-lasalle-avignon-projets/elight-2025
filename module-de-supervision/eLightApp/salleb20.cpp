#include "salleb20.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

salleB20::salleB20(QWidget *parent) : QWidget(parent) {
  setWindowTitle("salleB20");

  QLabel *titreB20 = new QLabel("Salle B20", this);
  QLabel *labelLogoeLight = new QLabel;
  QLabel *conso = new QLabel("Consommation d'énergie:");

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreB20->setStyleSheet("font-weight: 900; font-size: 90px;");
  conso->setStyleSheet("border: 1px solid black; background-color: #FFFF33;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreB20, Qt::AlignBaseline);
}
