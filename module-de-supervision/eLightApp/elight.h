#ifndef ELIGHT_H
#define ELIGHT_H

#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QVBoxLayout>

#define CHEMIN_RESSOURCE "ressource"

#define STYLE_APPLICATION "styleSheet.css"

QT_BEGIN_NAMESPACE
namespace Ui {
class eLight;
}
QT_END_NAMESPACE

class eLight : public QWidget {
  Q_OBJECT

public:
  eLight(QWidget *parent = nullptr);
  ~eLight();

  void chargeurFeuilleStyle();

private:
  Ui::eLight *ui;
};
#endif // ELIGHT_H
