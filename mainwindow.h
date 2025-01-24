#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DialogCamera.h"
#include <QTimer>
#include <QDebug>
#include <QRect>

#include "DialogRobot.h"
#include "Scheduler.h"
#include "Logger.h"
//#include "DialogStart.h"

//class DialogStart;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    DialogRobot* robot;
    Scheduler* scheduler;

    QVector<DialogCamera*>      ptemp;

private:
    Ui::MainWindow *ui;
    QTimer *timer;

//    DialogStart *dialogStart;

    QTime exit_time;
    QTime restart_time;
    QTime minimize_time;
    void changeTextColor();
    // Reference instead of pointer for the singleton
//    Scheduler& scheduler;


public slots:
    void onTimer();

private slots:
    void on_BTN_EXIT_released();
    void on_BTN_EXIT_pressed();
    void on_BTN_MINIMIZE_released();
    void on_BTN_MINIMIZE_pressed();
    void on_BTN_RESTART_pressed();
    void on_BTN_RESTART_released();
    void on_CALIBRATION_clicked();

};
#endif // MAINWINDOW_H
