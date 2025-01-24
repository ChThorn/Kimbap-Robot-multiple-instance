#ifndef DIALOGSTART_H
#define DIALOGSTART_H

#include <QDialog>
//#include "mainwindow.h"
#include "DialogCalibration.h"
#include "GlobalHeader.h"
#include <QDir>
#include <QFile>
#include <QDebug>

class MainWindow;

namespace Ui {
class DialogStart;
}

class DialogStart : public QDialog
{
    Q_OBJECT

public:
    explicit DialogStart(QWidget *parent = nullptr);
    ~DialogStart();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::DialogStart *ui;
//    MainWindow * mainWindow;
//    DialogCalibration *dialogCal;

    //---Load calibration check---//
    const QString CALIB_PATH = LOAD_CALIB;
    const QString CALIB_FILENAME = "calibration_data.txt";
    bool loadCalibrated();
//    void getFile();

private slots:
    void on_SKIP_clicked();
    void on_CALIBRATION_clicked();
};

#endif // DIALOGSTART_H
