#include "boitesegment.h"
#include "config.h"
#include <QDebug>

BoiteSegment::BoiteSegment(int segmentId, QWidget* parent) :
    QWidget(parent), idSegment(segmentId)
{
    QPixmap iconeLampe(QString(CHEMIN_RESSOURCES) + QString(ICONE_SEGMENT));

    imageLabel     = new QLabel(this);
    puissanceLabel = new QLabel(this);
    puissanceLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    imageLabel->setPixmap(
      iconeLampe.scaled(LARGEUR_ICONE, HAUTEUR_ICONE, Qt::KeepAspectRatio));

    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->addWidget(imageLabel);
    layout->addStretch();
    layout->addWidget(puissanceLabel);
    layout->addStretch();
    layout->setAlignment(imageLabel, Qt::AlignLeft);
    layout->setAlignment(puissanceLabel, Qt::AlignCenter);

    this->setAttribute(Qt::WA_StyledBackground, true);
}

void BoiteSegment::setPuissance(double nouvellePuissance)
{
    puissance = nouvellePuissance;

    puissanceLabel->setText(QString::number(nouvellePuissance, 'f', 2) + " W");

    if(nouvellePuissance == 0)
    {
        this->setStyleSheet("background-color: #ff5353; border-radius: 15px;");
    }
    else
    {
        this->setStyleSheet("background-color: #6bff6b; border-radius: 15px;");
    }
}

int BoiteSegment::getIdSegment()
{
    return idSegment;
}
