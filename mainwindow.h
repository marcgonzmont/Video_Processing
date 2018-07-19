#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QCloseEvent>
#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QDate>
#include <QFileInfo>
#include <QFile>
#include "opencv2/opencv.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

    void finishEvent();

    void checkArg(QString arg);

private slots:
    void on_inputPushButton_pressed();

    void on_outputPushButton_pressed();

    void on_processButton_pressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
