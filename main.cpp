#include "mainwindow.h"
#include"opencv2/opencv.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    using namespace cv;

    return a.exec();
}
