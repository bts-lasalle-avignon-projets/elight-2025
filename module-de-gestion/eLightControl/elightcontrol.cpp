/**
 * @file elightcontrol.cpp
 *
 * @brief Définition de la classe ELightControl
 * @author Soria Bonet Enzo
 * @version 1.0
 */

#include "elightcontrol.h"
#include <QDebug>

/**
 * @brief Constructeur de la classe ELightControl
 *
 * @fn ELightControl::ELightControl
 * @param parent L'adresse de l'objet parent, si nullptr ELightControl sera la
 * fenêtre principale de l'application
 */
ELightControl::ELightControl(QWidget* parent) : QWidget(parent)
{
    qDebug() << Q_FUNC_INFO << this;

    setWindowTitle(QString(APPLICATION) + QString(" v") + QString(VERSION));

#ifdef RASPBERRY_PI
    showFullScreen();
#endif
}

ELightControl::~ELightControl()
{
    qDebug() << Q_FUNC_INFO << this;
}
