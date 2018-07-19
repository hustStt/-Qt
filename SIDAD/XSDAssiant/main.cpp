#include "mainwindow.h"
#include <QApplication>

MainWindow* w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow();
    w->show();
    //return 0;
    return a.exec();
}
