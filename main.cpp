//#include "mainwindow.h"
#include "DialogStart.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w;
    DialogStart w;
    w.show();
    return a.exec();
}
