#ifndef INTERFACEHOMMEMACHINE_H
#define INTERFACEHOMMEMACHINE_H

#include <QtWidgets>

class InterfaceHommeMachine : public QWidget
{
    Q_OBJECT
  public:
    explicit InterfaceHommeMachine(QWidget* parent = nullptr);
    QStackedWidget* pageEmpilees;

  signals:
};

#endif // INTERFACEHOMMEMACHINE_H
