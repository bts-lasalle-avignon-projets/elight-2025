#include "boitesegment.h"
#include "config.h"
#include <QDebug>

BoiteSegment::BoiteSegment(int segmentId, QWidget* parent) :
    QWidget(parent), idSegment(segmentId)
{
    QPixmap iconeLampe(QString(CHEMIN_RESSOURCES) + QString(ICONE_SEGMENT));
    QFont   police;
    police.setPointSize(TAILLE_POLICE);

    imageLabel     = new QLabel(this);
    puissanceLabel = new QLabel(this);
    puissanceLabel->setStyleSheet("font-weight: bold;");
    puissanceLabel->setFont(police);

    imageLabel->setPixmap(iconeLampe.scaled(LARGEUR_ICONE_LAMPE,
                                            HAUTEUR_ICONE_LAMPE,
                                            Qt::KeepAspectRatio,
                                            Qt::FastTransformation));

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

void BoiteSegment::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit segmentClique(idSegment);
    }
}
