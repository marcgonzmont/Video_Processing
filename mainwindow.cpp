#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "opencv2/opencv.hpp"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//    ui->progressBar->setMinimum(0);
//    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_inputPushButton_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open input video", QDir::currentPath(), "Videos .avi (*.avi);; Videos .mp4 (*.mp4)");
    if(QFile::exists(filename))
    {
        ui->inputLineEdit->setText(filename);
    }
}

void MainWindow::on_outputPushButton_pressed()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->outputLineEdit->setText(dir);
        //insert video function
    }
}

void MainWindow::on_processButton_pressed()
{
    using namespace cv;
    using namespace std;
    ui->progressBar->setValue(0);

    QString video_name = ui->inputLineEdit->text();
    QString out_path = ui->outputLineEdit->text();
    QString file_name = ui->nameInLabel->text();
    QString ext = video_name.split('.', QString::SkipEmptyParts).at(1);
    QString date = QDate::currentDate().toString("d.M.yyyy");
    QString full_name = out_path + "/" + file_name + "." + ext;

    VideoCapture video;
    video.open(video_name.toStdString());
    if(video.isOpened())
    {
        int value = ui->progressBar->value();
        Mat frame;
        int n_frame = 0;
        double step = 0;
        int total_frames = video.get(CAP_PROP_FRAME_COUNT);
        int fourcc = video.get(CV_CAP_PROP_FOURCC);
        double fps = video.get(CV_CAP_PROP_FPS);
        int frame_width = video.get(CV_CAP_PROP_FRAME_WIDTH);
        int frame_height = video.get(CV_CAP_PROP_FRAME_HEIGHT);

        //        // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
        VideoWriter video_out(full_name.toStdString(), fourcc, fps, Size(frame_width,frame_height));


        while(true)
        {
            if(video.read(frame))
            {
                n_frame = video.get(CV_CAP_PROP_POS_FRAMES);
                QString text1 = "Frame: " + QString::number(n_frame);
                QString text2 = "Date: " + date;
                putText(frame, text1.toStdString(), cvPoint(15, 15), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,255,0), 1);
                putText(frame, text2.toStdString(), cvPoint(15, 35), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(0,255,0), 1);
                video_out.write(frame);
                if(ui->showButton->isChecked())
                {
                    imshow("Processing", frame);
                    waitKey(20);
                    char c = (char)waitKey(33);
                    if( c == 27 ) break;
                }
//                value = ui->progressBar->value();
//                step = ((double)total_frames / 100.0) / 100.0;
//                ui->progressBar->setValue((int)(value + step));
                ui->progressBar->setValue((int)((double)n_frame / (double)total_frames * 100));
            }
            else
            {
                break;
            }
        }
        video_out.release();
    }
    video.release();
    destroyAllWindows();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int result = QMessageBox::warning(this, "Exit", "Confirm exit?", QMessageBox::Yes, QMessageBox::No);
    if(result == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

