/********************************************************************************
** Form generated from reading UI file 'DialogCalibration.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGCALIBRATION_H
#define UI_DIALOGCALIBRATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogCalibration
{
public:
    QPushButton *INTRINSIC;
    QPushButton *EXTRINSIC;
    QLabel *label;
    QPushButton *cancelButton;

    void setupUi(QDialog *DialogCalibration)
    {
        if (DialogCalibration->objectName().isEmpty())
            DialogCalibration->setObjectName(QString::fromUtf8("DialogCalibration"));
        DialogCalibration->resize(430, 180);
        INTRINSIC = new QPushButton(DialogCalibration);
        INTRINSIC->setObjectName(QString::fromUtf8("INTRINSIC"));
        INTRINSIC->setGeometry(QRect(30, 50, 151, 61));
        EXTRINSIC = new QPushButton(DialogCalibration);
        EXTRINSIC->setObjectName(QString::fromUtf8("EXTRINSIC"));
        EXTRINSIC->setGeometry(QRect(240, 50, 151, 61));
        label = new QLabel(DialogCalibration);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(70, 10, 291, 31));
        label->setAlignment(Qt::AlignCenter);
        cancelButton = new QPushButton(DialogCalibration);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(30, 130, 131, 41));

        retranslateUi(DialogCalibration);

        QMetaObject::connectSlotsByName(DialogCalibration);
    } // setupUi

    void retranslateUi(QDialog *DialogCalibration)
    {
        DialogCalibration->setWindowTitle(QCoreApplication::translate("DialogCalibration", "Dialog", nullptr));
        INTRINSIC->setText(QCoreApplication::translate("DialogCalibration", "Intrinsic", nullptr));
        EXTRINSIC->setText(QCoreApplication::translate("DialogCalibration", "Extrinsic", nullptr));
        label->setText(QCoreApplication::translate("DialogCalibration", "Select Calibration Type", nullptr));
        cancelButton->setText(QCoreApplication::translate("DialogCalibration", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogCalibration: public Ui_DialogCalibration {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGCALIBRATION_H
