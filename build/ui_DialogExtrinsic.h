/********************************************************************************
** Form generated from reading UI file 'DialogExtrinsic.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGEXTRINSIC_H
#define UI_DIALOGEXTRINSIC_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DialogExtrinsic
{
public:
    QLabel *statusLabel;
    QPushButton *BACK;
    QPushButton *TOGGLE;
    QComboBox *cameraTypeCombo;
    QPushButton *DELETE;
    QListWidget *imageList;
    QGraphicsView *graphicsView;
    QLabel *BANNER;

    void setupUi(QDialog *DialogExtrinsic)
    {
        if (DialogExtrinsic->objectName().isEmpty())
            DialogExtrinsic->setObjectName(QString::fromUtf8("DialogExtrinsic"));
        DialogExtrinsic->resize(882, 590);
        statusLabel = new QLabel(DialogExtrinsic);
        statusLabel->setObjectName(QString::fromUtf8("statusLabel"));
        statusLabel->setGeometry(QRect(670, 180, 191, 71));
        BACK = new QPushButton(DialogExtrinsic);
        BACK->setObjectName(QString::fromUtf8("BACK"));
        BACK->setGeometry(QRect(40, 540, 111, 41));
        TOGGLE = new QPushButton(DialogExtrinsic);
        TOGGLE->setObjectName(QString::fromUtf8("TOGGLE"));
        TOGGLE->setGeometry(QRect(270, 540, 111, 41));
        cameraTypeCombo = new QComboBox(DialogExtrinsic);
        cameraTypeCombo->setObjectName(QString::fromUtf8("cameraTypeCombo"));
        cameraTypeCombo->setGeometry(QRect(670, 50, 181, 25));
        DELETE = new QPushButton(DialogExtrinsic);
        DELETE->setObjectName(QString::fromUtf8("DELETE"));
        DELETE->setGeometry(QRect(470, 540, 111, 41));
        imageList = new QListWidget(DialogExtrinsic);
        imageList->setObjectName(QString::fromUtf8("imageList"));
        imageList->setGeometry(QRect(650, 300, 231, 231));
        graphicsView = new QGraphicsView(DialogExtrinsic);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(10, 50, 640, 480));
        BANNER = new QLabel(DialogExtrinsic);
        BANNER->setObjectName(QString::fromUtf8("BANNER"));
        BANNER->setGeometry(QRect(220, 10, 221, 31));
        BANNER->setAlignment(Qt::AlignCenter);

        retranslateUi(DialogExtrinsic);

        QMetaObject::connectSlotsByName(DialogExtrinsic);
    } // setupUi

    void retranslateUi(QDialog *DialogExtrinsic)
    {
        DialogExtrinsic->setWindowTitle(QCoreApplication::translate("DialogExtrinsic", "Dialog", nullptr));
        statusLabel->setText(QCoreApplication::translate("DialogExtrinsic", "TextLabel", nullptr));
        BACK->setText(QCoreApplication::translate("DialogExtrinsic", "Back", nullptr));
        TOGGLE->setText(QCoreApplication::translate("DialogExtrinsic", "Capture", nullptr));
        DELETE->setText(QCoreApplication::translate("DialogExtrinsic", "Delete", nullptr));
        BANNER->setText(QCoreApplication::translate("DialogExtrinsic", "Extrinsic Calibration", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogExtrinsic: public Ui_DialogExtrinsic {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGEXTRINSIC_H
