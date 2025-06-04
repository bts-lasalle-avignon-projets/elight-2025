#include "boitesegment.h"
#include "config.h"
#include <QDebug>

BoiteSegment::BoiteSegment(int segmentId, QWidget* parent) :
    QWidget(parent), idSegment(segmentId)
{
    QPixmap iconeLampe(QString(CHEMIN_RESSOURCES) + QString(ICONE_SEGMENT));
    QFont   police;
    police.setPointSize(TAILLE_POLICE);

    this->setObjectName("segment");

    imageLabel = new QLabel(this);
    imageLabel->setObjectName("imageLabelSegment");
    puissanceLabel = new QLabel(this);
    puissanceLabel->setObjectName("puissanceLabelSegment");
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
        this->setStyleSheet("#segment { "
                            "background-color: #ff5353;"
                            "border-radius: 9px; "
                            "border: 2px solid #d93025; }"
                            "#imageLabelSegment, #puissanceLabelSegment { "
                            "background-color: #ff5353; }");
    }
    else
    {
        this->setStyleSheet("#segment { "
                            "background-color: #a6e36e;"
                            "border-radius: 9px; "
                            "border: 2px solid #78c850; }"
                            "#imageLabelSegment, #puissanceLabelSegment { "
                            "background-color: #a6e36e; }");
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
