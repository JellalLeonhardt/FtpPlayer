#include "mainwindow.h"
#include "loginwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();

    loginWindow l;
    l.setWindowTitle("登录");
    l.show();

    return a.exec();
}
