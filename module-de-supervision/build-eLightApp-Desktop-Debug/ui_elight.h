/********************************************************************************
** Form generated from reading UI file 'elight.ui'
**
** Created by: Qt User Interface Compiler version 5.12.8
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ELIGHT_H
#define UI_ELIGHT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_eLight
{
public:

    void setupUi(QWidget *eLight)
    {
        if (eLight->objectName().isEmpty())
            eLight->setObjectName(QString::fromUtf8("eLight"));
        eLight->resize(800, 600);

        retranslateUi(eLight);

        QMetaObject::connectSlotsByName(eLight);
    } // setupUi

    void retranslateUi(QWidget *eLight)
    {
        eLight->setWindowTitle(QApplication::translate("eLight", "eLight", nullptr));
    } // retranslateUi

};

namespace Ui {
    class eLight: public Ui_eLight {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ELIGHT_H
