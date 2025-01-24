#include "DialogStart.h"
#include "ui_DialogStart.h"
#include <QMessageBox>
#include <QDesktopWidget>
#include <QStyle>
#include <QApplication>
#include <QScreen>
#include "mainwindow.h"

DialogStart::DialogStart(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogStart)
{
    ui->setupUi(this);
//    mainWindow = nullptr;
//    dialogCal = nullptr;

    // Set window flags for transparency
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Style for Skip button (using blue theme)
    QString skipButtonStyle =
        "QPushButton {"
        "    background-color: #2196F3;"  // Blue
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 15px 30px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #1976D2;"  // Darker blue on hover
        "}"
        "QPushButton:pressed {"
        "    background-color: #0D47A1;"  // Even darker when pressed
        "}";

    // Style for Calibration button (using green theme)
    QString calibrationButtonStyle =
        "QPushButton {"
        "    background-color: #4CAF50;"  // Green
        "    color: white;"
        "    border: none;"
        "    border-radius: 10px;"
        "    padding: 15px 30px;"
        "    font-size: 16px;"
        "    font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "    background-color: #388E3C;"  // Darker green on hover
        "}"
        "QPushButton:pressed {"
        "    background-color: #1B5E20;"  // Even darker when pressed
        "}";

    // Apply different styles to each button
    ui->SKIP->setStyleSheet(skipButtonStyle);
    ui->CALIBRATION->setStyleSheet(calibrationButtonStyle);

    // Get the screen geometry
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();

    // Set the dialog size to cover the full screen
    resize(screenGeometry.width(), screenGeometry.height());
    move(0, 0);

    // Calculate center position for buttons
    int centerX = (screenGeometry.width() - ui->SKIP->width() - ui->CALIBRATION->width() - 70) / 2;
    int centerY = (screenGeometry.height() - ui->SKIP->height()) / 2;

    // Position buttons
    ui->SKIP->move(centerX, centerY);
    ui->CALIBRATION->move(centerX + ui->SKIP->width() + 70, centerY);
    show();
    raise();
    activateWindow();
    setFocus();
}

void DialogStart::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    // Create darker semi-transparent background for full screen
    painter.fillRect(rect(), QColor(0, 0, 0, 200));

    // Calculate the center panel position and size
    int panelWidth = ui->SKIP->width() + ui->CALIBRATION->width() + 140;  // Add padding
    int panelHeight = ui->SKIP->height() + 40;  // Add padding
    int x = (width() - panelWidth) / 2;
    int y = (height() - panelHeight) / 2;

    QRect centerRect(x, y, panelWidth, panelHeight);

    // Add subtle shadow
    QColor shadowColor(0, 0, 0, 40);
    for(int i = 0; i < 8; i++) {
        painter.setPen(Qt::NoPen);
        painter.setBrush(shadowColor);
        painter.drawRoundedRect(centerRect.adjusted(-i, -i, i, i), 12, 12);
    }

    // Create gradient for panel background
    QLinearGradient gradient(centerRect.topLeft(), centerRect.bottomRight());
    gradient.setColorAt(0, QColor(255, 255, 255, 250));
    gradient.setColorAt(1, QColor(240, 240, 240, 250));

    // Draw main panel with white background
    painter.setBrush(gradient);
    painter.setPen(QPen(QColor(220, 220, 220), 1));
    painter.drawRoundedRect(centerRect, 8, 8);

    QDialog::paintEvent(event);
}

DialogStart::~DialogStart()
{
//    if(mainWindow)
//    {
//        delete mainWindow;
//        mainWindow = nullptr;
//    }

//    if(dialogCal)
//    {
//        delete dialogCal;
//        dialogCal = nullptr;
//    }
    delete ui;
}

void DialogStart::on_SKIP_clicked()
{
    // Create and show MainWindow only when Skip is clicked
    if(!loadCalibrated())
    {
        QMessageBox::warning(this, "Warning", "No Calibration file found.");
    }
    else
    {
        auto mainWindow = new MainWindow();
        mainWindow->show();
        this->close();
    }
}

void DialogStart::on_CALIBRATION_clicked()
{
    QMessageBox::information(this, "Calibration", "Calibration process would start here.");
//    this->close();
    auto dialogCal = new DialogCalibration();
    dialogCal->show();
    this->close();
}

bool DialogStart::loadCalibrated()
{
    // Check if directory exists
    QDir dir(CALIB_PATH);
    if(!dir.exists())
    {
        qDebug() << "Calibration directory does not exist:" << CALIB_PATH;
        return false;
    }

    // Get list of files in directory
    QStringList files = dir.entryList(QDir::Files);
    if(files.isEmpty())
    {
        qDebug() << "No calibration files found in:" << CALIB_PATH;
        return false;
    }

    for(const QString& file : files)
    {
        // Be more specific with the file name you're looking for
        if(file == "calibrated_extrinsic.jsp" || file == "calibrated_extrinsic.txt")  // Replace with your exact file name
        {
            QString fullPath = CALIB_PATH + "/" + file;
            QFile calibFile(fullPath);
            if(calibFile.exists())
            {
                qDebug() << "Found calibration file:" << fullPath;
                return true;
            }
        }
    }

    qDebug() << "No valid calibration files found";
    return false;
}

//void DialogStart::getFile()
//{

//}
