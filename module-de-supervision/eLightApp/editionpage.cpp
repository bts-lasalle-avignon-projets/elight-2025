#include "editionpage.h"

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QtWidgets>

editionPage::editionPage(QWidget *parent) : QWidget(parent) {
  setWindowTitle("edition");

  QLabel *titreEdit = new QLabel("Édition", this);
  QLabel *labelLogoeLight = new QLabel;

  QString cheminRessource = "./ressource/";
  QPixmap logoeLight(cheminRessource + "logo-elight.png");

  this->setStyleSheet("background-color: #FFFFFF;");
  titreEdit->setStyleSheet("font-weight: 900; font-size: 90px;");

  QVBoxLayout *layout = new QVBoxLayout(this);
  QHBoxLayout *logotitre = new QHBoxLayout;

  labelLogoeLight->setPixmap(logoeLight);

  layout->addLayout(logotitre);

  logotitre->addWidget(labelLogoeLight);
  logotitre->addWidget(titreEdit, Qt::AlignBaseline);
}
