/**
 * @file main.cpp
 *
 * @brief Programme principal
 * @author Soria Bonet Enzo
 * @version 1.0
 */
#include "elightcontrol.h"
#include <QApplication>

/**
 * @brief Programme principal
 * @details Crée et affiche la fenêtre principale de l'application eLightControl
 *
 * @fn main
 * @param argc
 * @param argv[]
 * @return int
 *
 */
int main(int argc, char* argv[])
{
    QApplication  a(argc, argv);
    ELightControl eLightControl;

    eLightControl.show();

    return a.exec();
}
