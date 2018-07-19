#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "opencv2/opencv.hpp"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * When input button is pressed list the files in file explorer with the selected extension
 * and if the file exists put the full path into lineEdit
*/
void MainWindow::on_inputPushButton_pressed()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open input video", QDir::currentPath(),
                                                    "Videos .avi (*.avi);; "
                                                    "Videos .mp4 (*.mp4);; "
                                                    "Videos .mov (*.mov);; "
                                                    "Videos .mkv (*.mkv)");
    if(QFile::exists(filename))
    {
        ui->inputLineEdit->setText(filename);
    }
}

/*
 * When output button is pressed open the file explorer for select a folder and if it exists
 * put the full path into lineEdit
*/
void MainWindow::on_outputPushButton_pressed()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    QDir::currentPath(),
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if(!dir.isEmpty())
    {
        ui->outputLineEdit->setText(dir);
    }
}

/*
 * When process button is pressed initialize the value of progressBar and get the content of each lineEdit.
 * Open the input video, get each frame and put into it the number frame and the date of the processing.
 * At last, each frame is written in a new video file
*/
void MainWindow::on_processButton_pressed()
{
    using namespace cv;
    ui->progressBar->setValue(0);

    QString video_name = ui->inputLineEdit->text();
    QString out_path = ui->outputLineEdit->text();
    QString file_name = ui->nameInLabel->text();
    QString date = QDate::currentDate().toString("d.M.yyyy");
    MainWindow::checkArg(video_name);
    MainWindow::checkArg(out_path);

    if(file_name.isEmpty())
    {
        file_name = QFileInfo(video_name).fileName();
        file_name = file_name.split('.', QString::SkipEmptyParts).at(0) + "_processed";
    }
    QString ext = video_name.split('.', QString::SkipEmptyParts).at(1);
    QString full_name = out_path + "/" + file_name + "." + ext;

    VideoCapture video;
    video.open(video_name.toStdString());
    if(video.isOpened())
    {
        Mat frame;
        int n_frame = 0;
        int total_frames = video.get(CAP_PROP_FRAME_COUNT);
        int fourcc = video.get(CV_CAP_PROP_FOURCC);
        double fps = video.get(CV_CAP_PROP_FPS);
        int frame_width = video.get(CV_CAP_PROP_FRAME_WIDTH);
        int frame_height = video.get(CV_CAP_PROP_FRAME_HEIGHT);

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
    MainWindow::finishEvent();
}

/*
 * When the process is finished launch an information message
*/
void MainWindow::finishEvent()
{
    QMessageBox::warning(this, "Process finished", "The process has finished", QMessageBox::Ok);
}

/*
 * Check if the argument is empty (paths). If it is launch an information message
*/
void MainWindow::checkArg(QString arg)
{
    if(arg.isEmpty())
    {
        QMessageBox::warning(this, "ERROR", "Please, check the INPUT FILE or OUTPUT PATH is empty.", QMessageBox::Ok);
    }

}

/*
 * When user close the application launch an information message
*/
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

