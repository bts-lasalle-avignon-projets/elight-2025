#include "editionsalle.h"

#include <QDebug>

EditionSalle::EditionSalle(QWidget* parent) : QWidget(parent)
{
    QPushButton* boutonFermeture = new QPushButton("Fermer", this);

    QVBoxLayout* layout = new QVBoxLayout(this);

    layout->addWidget(boutonFermeture);

    connect(boutonFermeture,
            &QPushButton::clicked,
            this,
            &EditionSalle::fermerFenetre);

    this->setStyleSheet("background-color: #FFFFFF;");
#ifdef RASPBERRY_PI
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Dialog); // Ajouter Qt::WindowStaysOnTopHint*/
#else
    setWindowFlags(Qt::Dialog);
#endif

    setWindowModality(Qt::WindowModal);
}

void EditionSalle::fermerFenetre()
{
    this->close();
}
