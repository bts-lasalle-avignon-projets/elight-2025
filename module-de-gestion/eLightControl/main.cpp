#include "elightcontrol.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    eLightControl w;
    w.show();
    return a.exec();
}
