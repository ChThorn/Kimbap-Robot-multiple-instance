/********************************************************************************
** Form generated from reading UI file 'DialogIntrinsic.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGINTRINSIC_H
#define UI_DIALOGINTRINSIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogIntrinsic
{
public:
    QGraphicsView *graphicsView;
    QPushButton *TOGGLE;
    QPushButton *BACK;
    QPushButton *DELETE;
    QLabel *statusLabel;
    QListWidget *imageList;
    QComboBox *cameraTypeCombo;
    QLabel *BANNER;

    void setupUi(QDialog *DialogIntrinsic)
    {
        if (DialogIntrinsic->objectName().isEmpty())
            DialogIntrinsic->setObjectName(QString::fromUtf8("DialogIntrinsic"));
        DialogIntrinsic->resize(882, 595);
        graphicsView = new QGraphicsView(DialogIntrinsic);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 50, 640, 480));
        TOGGLE = new QPushButton(DialogIntrinsic);
        TOGGLE->setObjectName(QString::fromUtf8("TOGGLE"));
        TOGGLE->setGeometry(QRect(270, 550, 111, 41));
        BACK = new QPushButton(DialogIntrinsic);
        BACK->setObjectName(QString::fromUtf8("BACK"));
        BACK->setGeometry(QRect(40, 550, 111, 41));
        DELETE = new QPushButton(DialogIntrinsic);
        DELETE->setObjectName(QString::fromUtf8("DELETE"));
        DELETE->setGeometry(QRect(470, 550, 111, 41));
        statusLabel = new QLabel(DialogIntrinsic);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(670, 180, 191, 71));
        imageList = new QListWidget(DialogIntrinsic);
        imageList->setObjectName(QString::fromUtf8("imageList"));
        imageList->setGeometry(QRect(650, 300, 231, 231));
        cameraTypeCombo = new QComboBox(DialogIntrinsic);
        cameraTypeCombo->setObjectName(QString::fromUtf8("cameraTypeCombo"));
        cameraTypeCombo->setGeometry(QRect(670, 50, 181, 25));
        BANNER = new QLabel(DialogIntrinsic);
        BANNER->setObjectName(QString::fromUtf8("BANNER"));
        BANNER->setGeometry(QRect(220, 10, 231, 41));
        BANNER->setAlignment(Qt::AlignCenter);

        retranslateUi(DialogIntrinsic);

        QMetaObject::connectSlotsByName(DialogIntrinsic);
    } // setupUi

    void retranslateUi(QDialog *DialogIntrinsic)
    {
        DialogIntrinsic->setWindowTitle(QCoreApplication::translate("DialogIntrinsic", "Dialog", nullptr));
        TOGGLE->setText(QCoreApplication::translate("DialogIntrinsic", "Capture", nullptr));
        BACK->setText(QCoreApplication::translate("DialogIntrinsic", "Back", nullptr));
        DELETE->setText(QCoreApplication::translate("DialogIntrinsic", "Delete", nullptr));
        statusLabel->setText(QCoreApplication::translate("DialogIntrinsic", "TextLabel", nullptr));
        BANNER->setText(QCoreApplication::translate("DialogIntrinsic", "Intrinsic Calibration", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogIntrinsic: public Ui_DialogIntrinsic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGINTRINSIC_H
