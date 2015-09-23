#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <iostream>

#include <QVector>
#include <QRgb>


using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->open->setShortcut(Qt::Key_O);
    ui->points->setShortcut(Qt::Key_P);
    ui->vor->setShortcut(Qt::Key_V);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_clicked()
{
    double scale;
    QString fileName = QFileDialog::getOpenFileName(this,
                      tr("Open Image"),".",
                      tr("Image Files(*.png *.jpg *.jpeg *.bmp)"));
    if(fileName == NULL)
        return ;

    Mat image0 = imread(fileName.toStdString().c_str());

    if(image0.rows >= HEIGHT || image0.cols >= WIDTH)
    {
        double hr = (double)HEIGHT/(double)image0.rows;
        double wc = (double)WIDTH/(double)image0.cols;
        scale = hr > wc ? wc : hr;
        cv::resize(image0,image,Size(0,0),scale,scale,INTER_LINEAR);
    }
    else
        image0.copyTo(image);

    cvtColor(image,image,CV_BGR2RGB);
    cvtColor(image,gray,CV_RGB2GRAY);
    QImage img = QImage((const unsigned char*)(image.data),
                        image.cols,image.rows,image.cols*image.channels(),
                        QImage::Format_RGB888);
    ui->srcLabel->setPixmap(QPixmap::fromImage(img));
    ui->srcLabel->resize(ui->srcLabel->pixmap()->size());

    QImage img0 = QImage((const unsigned char*)(gray.data),
                         gray.cols,gray.rows,gray.cols*gray.channels(),
                         QImage::Format_Indexed8);

    QVector<QRgb> colortable;
    for(int i=0;i<256;i++)
        colortable.push_back(qRgb(i,i,i));
    img0.setColorTable(colortable);
    ui->dstLabel->setPixmap(QPixmap::fromImage(img0));
    ui->dstLabel->resize(ui->dstLabel->pixmap()->size());

}

void MainWindow::on_points_clicked()
{
    ui->dstLabel->setPoints(gray);
}

void MainWindow::on_vor_clicked()
{
    ui->dstLabel->voronoi();
}
