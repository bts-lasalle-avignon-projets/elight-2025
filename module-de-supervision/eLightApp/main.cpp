/**
 * @file main.cpp
 *
 * @brief Programme principal
 * @author Blondel Joshua
 * @version 1.0
 */
#include "elight.h"
#include <QApplication>

/**
 * @brief Programme principal
 * @details Crée et affiche la fenêtre principale de l'application eLightApp
 *
 * @fn main
 * @param argc
 * @param argv[]
 * @return int
 *
 */
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    ELight       eLight;

    eLight.show();

    return a.exec();
}
