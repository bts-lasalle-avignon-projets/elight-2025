#include "boitesegment.h"
#include "elightcontrol.h"

BoiteSegment::BoiteSegment(int segmentId, QWidget* parent) :
    QWidget(parent), idSegment(segmentId)
{
    QPixmap iconeLampe(QString(CHEMIN_RESSOURCES) + QString(ICONE_SEGMENT));

    imageLabel        = new QLabel(this);
    consommationLabel = new QLabel(this);
    consommationLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    imageLabel->setPixmap(
      iconeLampe.scaled(LARGEUR_ICONE, HAUTEUR_ICONE, Qt::KeepAspectRatio));

    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->addWidget(imageLabel);
    layout->addStretch();
    layout->addWidget(consommationLabel);
    layout->addStretch();
    layout->setAlignment(imageLabel, Qt::AlignLeft);
    layout->setAlignment(consommationLabel, Qt::AlignCenter);

    this->setAttribute(Qt::WA_StyledBackground, true);
}

void BoiteSegment::setConsommation(double nouvelleConsommation)
{
    consommation = nouvelleConsommation;

    consommationLabel->setText(QString::number(nouvelleConsommation, 'f', 2) +
                               " W");
    if(nouvelleConsommation == 0)
    {
        this->setStyleSheet("background-color: #ff5353; border-radius: 15px;");
    }
    else
    {
        this->setStyleSheet("background-color: #6bff6b; border-radius: 15px;");
    }
}
