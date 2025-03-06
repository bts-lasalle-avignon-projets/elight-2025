#include "elightcontrol.h"
#include "ui_elightcontrol.h"

eLightControl::eLightControl(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::eLightControl)
{
    ui->setupUi(this);
}

eLightControl::~eLightControl()
{
    delete ui;
}

