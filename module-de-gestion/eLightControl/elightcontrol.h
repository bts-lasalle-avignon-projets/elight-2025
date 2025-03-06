#ifndef ELIGHTCONTROL_H
#define ELIGHTCONTROL_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class eLightControl; }
QT_END_NAMESPACE

class eLightControl : public QWidget
{
    Q_OBJECT

public:
    eLightControl(QWidget *parent = nullptr);
    ~eLightControl();

private:
    Ui::eLightControl *ui;
};
#endif // ELIGHTCONTROL_H
