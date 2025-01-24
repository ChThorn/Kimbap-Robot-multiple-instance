#include "DialogCalibration.h"
#include "ui_DialogCalibration.h"
#include <QMessageBox>
#include <QScreen>
#include <QPainter>
#include "DialogIntrinsic.h"
#include "DialogExtrinsic.h"
#include "DialogStart.h"

DialogCalibration::DialogCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCalibration)
{
    ui->setupUi(this);

//    dialogStart = nullptr;
//    dialogIntrinsic = nullptr;
//    dialogExtrinsic = nullptr;

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Style for Start button (using green theme)
    QString startButtonStyle =
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 15px 30px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #388E3C;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1B5E20;"
        "}";

    // Style for Cancel button (using red theme)
    QString cancelButtonStyle =
        "QPushButton {"
        "    background-color: #F44336;"
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 15px 30px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #D32F2F;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #B71C1C;"
        "}";

    // Style for labels
    QString labelStyle =
        "QLabel {"
        "    color: #333333;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "}";

    // Apply styles
    ui->INTRINSIC->setStyleSheet(startButtonStyle);
    ui->EXTRINSIC->setStyleSheet(startButtonStyle);
    ui->cancelButton->setStyleSheet(cancelButtonStyle);
    ui->label->setStyleSheet(labelStyle);

    // Get screen geometry and set window size
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    resize(screenGeometry.width(), screenGeometry.height());
    move(0, 0);

    // Calculate positions
    int centerX = screenGeometry.width() / 2;
    int centerY = screenGeometry.height() / 2;

    // Position label
    ui->label->move(centerX - 140, centerY - 90);

    // Position buttons
    int buttonY = centerY - 40;  // Buttons below label
    ui->INTRINSIC->move(centerX - 170, buttonY);
    ui->EXTRINSIC->move(centerX + 30, buttonY);
    ui->cancelButton->move(centerX - 170, buttonY + 80);

    show();
    raise();
    activateWindow();
    setFocus();
}

DialogCalibration::~DialogCalibration()
{
//    if(dialogIntrinsic)
//    {
//        delete dialogIntrinsic;
//        dialogIntrinsic = nullptr;
//    }

//    if(dialogExtrinsic)
//    {
//        delete dialogExtrinsic;
//        dialogExtrinsic = nullptr;
//    }

//    if(dialogStart)
//    {
//        delete dialogStart;
//        dialogStart = nullptr;
//    }
    delete ui;
}

void DialogCalibration::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.fillRect(rect(), QColor(0, 0, 0, 200));

    // Calculate panel size to fit all three buttons
    int panelWidth = ui->INTRINSIC->width() * 3 + 50;  // Width for 3 buttons plus spacing
    int panelHeight = ui->INTRINSIC->height() + 140;    // Height plus padding
    int x = (width() - panelWidth) / 2;
    int y = (height() - panelHeight) / 2;

    QRect centerRect(x, y, panelWidth, panelHeight);

    // Add shadow
    QColor shadowColor(0, 0, 0, 140);
    for(int i = 0; i < 8; i++) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(shadowColor);
        painter.drawRoundedRect(centerRect.adjusted(-i, -i, i, i), 12, 12);
    }

    // Panel background
    QLinearGradient gradient(centerRect.topLeft(), centerRect.bottomRight());
    gradient.setColorAt(0, QColor(255, 255, 255, 250));
    gradient.setColorAt(1, QColor(240, 240, 240, 250));

    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    painter.drawRoundedRect(centerRect, 8, 8);

    QDialog::paintEvent(event);
}

void DialogCalibration::on_INTRINSIC_clicked()
{
    QMessageBox::information(this, "Intrinsic Calibration", "Starting intrinsic calibration process...");
    // TODO: Add intrinsic calibration logic
//    accept();
    auto dialogIntrinsic = new DialogIntrinsic();
    dialogIntrinsic->show();
    this->close();

}

void DialogCalibration::on_EXTRINSIC_clicked()
{
    QMessageBox::information(this, "Extrinsic Calibration", "Starting extrinsic calibration process...");
    // TODO: Add extrinsic calibration logic
//    accept();
    auto dialogExtrinsic = new DialogExtrinsic();
    dialogExtrinsic->show();
    this->close();
}

void DialogCalibration::on_cancelButton_clicked()
{
//    reject();
    auto dialogStart = new DialogStart();
    dialogStart->show();
    this->close();
}
