#include "historique.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

Historique::Historique(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Historique");

  QLabel *titreHist =
      new QLabel("Historique de la consommation d'énergie", this);
  QLabel *labelLogoeLight = new QLabel;

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreHist->setStyleSheet("font-weight: 900; font-size: 90px;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreHist, Qt::AlignBaseline);
}
