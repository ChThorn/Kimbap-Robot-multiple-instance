/********************************************************************************
** Form generated from reading UI file 'DialogStart.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGSTART_H
#define UI_DIALOGSTART_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogStart
{
public:
    QPushButton *SKIP;
    QPushButton *CALIBRATION;

    void setupUi(QDialog *DialogStart)
    {
        if (DialogStart->objectName().isEmpty())
            DialogStart->setObjectName(QString::fromUtf8("DialogStart"));
        DialogStart->resize(430, 180);
        SKIP = new QPushButton(DialogStart);
        SKIP->setObjectName(QString::fromUtf8("SKIP"));
        SKIP->setGeometry(QRect(50, 60, 131, 61));
        CALIBRATION = new QPushButton(DialogStart);
        CALIBRATION->setObjectName(QString::fromUtf8("CALIBRATION"));
        CALIBRATION->setGeometry(QRect(230, 60, 151, 61));

        retranslateUi(DialogStart);

        QMetaObject::connectSlotsByName(DialogStart);
    } // setupUi

    void retranslateUi(QDialog *DialogStart)
    {
        DialogStart->setWindowTitle(QCoreApplication::translate("DialogStart", "Dialog", nullptr));
        SKIP->setText(QCoreApplication::translate("DialogStart", "Continue", nullptr));
        CALIBRATION->setText(QCoreApplication::translate("DialogStart", "Calibration", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogStart: public Ui_DialogStart {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGSTART_H
