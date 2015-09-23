#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv.hpp>

#include <QPointF>

#include <iostream>
#include <vector>

using namespace std;

#define WIDTH 600
#define HEIGHT 600

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_clicked();

    void on_points_clicked();

    void on_vor_clicked();

private:
    Ui::MainWindow *ui;

    cv::Mat image;

    cv::Mat gray;

    vector<QPointF> points;
};

#endif // MAINWINDOW_H
