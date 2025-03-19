#include "elight.h"

#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  eLight w;
  w.show();
  return a.exec();
}
