#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

#include "opencvqtconverter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_actionOpen_triggered(void);
    void on_actionClear_triggered(void);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_slider_actionTriggered(int action);

    void on_image_moments_clicked();

private:
    Ui::MainWindow *ui;
    RNG rng;
    int TreshHold,deger;
    Mat src, canny_output,src_gray,drawing;
    QImage im1,im2;
    QPixmap pix1, pix2;
    QGraphicsScene sahne1, sahne2;

    OpencvQtConverter conv;

};

#endif // MAINWINDOW_H
