#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered(void){
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/", tr("Image Files (*.png *.jpg *.bmp *.jpeg)"));

    im1.load( fileName );

     src = conv.QImage2Mat( im1 );

     cvtColor( src, src_gray, COLOR_BGR2GRAY );
     blur( src_gray, src_gray, Size(3,3) );

     im1 = conv.Mat2QImage(src_gray);


    pix1 = QPixmap::fromImage( im1 );
    sahne1.addPixmap( pix1 );

    ui->DefaultImage->setScene(&sahne1);
}
void MainWindow::on_actionClear_triggered(void){

    sahne1.clear();
    sahne2.clear();
    ui->textEdit->clear();

}

void MainWindow::on_pushButton_clicked()
{

     TreshHold = ui->slider->value();

       Canny( src_gray, canny_output, TreshHold, TreshHold*2 );
       vector<vector<Point> > contours;
       findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
       vector<vector<Point> > contours_poly( contours.size() );
       vector<Rect> boundRect( contours.size() );
       vector<Point2f>centers( contours.size() );
       vector<float>radius( contours.size() );
       for( size_t i = 0; i < contours.size(); i++ )
       {
           approxPolyDP( contours[i], contours_poly[i], 3, true );
           boundRect[i] = boundingRect( contours_poly[i] );
           minEnclosingCircle( contours_poly[i], centers[i], radius[i] );
       }
       drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
       for( size_t i = 0; i< contours.size(); i++ )
       {
           Scalar color = Scalar( rng.uniform(0,256), rng.uniform(0,256), rng.uniform(0,256) );
           drawContours( drawing, contours_poly, (int)i, color );
           rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2 );
           circle( drawing, centers[i], (int)radius[i], color, 2 );
       }


   im2 = conv.Mat2QImage(drawing);


    pix2 = QPixmap::fromImage(im2);
    sahne2.addPixmap( pix2 );

    ui->graphicsView_2->setScene( &sahne2 );



}

void MainWindow::on_pushButton_2_clicked()
{
    TreshHold = ui->slider->value();
    rng(12345);
        Canny( src_gray, canny_output, TreshHold, TreshHold*2 );
        vector<vector<Point> > contours;
        findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );
        vector<RotatedRect> minRect( contours.size() );
        vector<RotatedRect> minEllipse( contours.size() );
        for( size_t i = 0; i < contours.size(); i++ )
        {
            minRect[i] = minAreaRect( contours[i] );
            if( contours[i].size() > 5 )
            {
                minEllipse[i] = fitEllipse( contours[i] );
            }
        }
        drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( size_t i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0,256), rng.uniform(0,256), rng.uniform(0,256) );
            // contour
            drawContours( drawing, contours, (int)i, color );
            // ellipse
            ellipse( drawing, minEllipse[i], color, 2 );
            // rotated rectangle
            Point2f rect_points[4];
            minRect[i].points( rect_points );
            for ( int j = 0; j < 4; j++ )
            {
                line( drawing, rect_points[j], rect_points[(j+1)%4], color );
            }
        }
        im2 = conv.Mat2QImage(drawing);


         pix2 = QPixmap::fromImage(im2);
         sahne2.addPixmap( pix2 );

         ui->graphicsView_2->setScene( &sahne2 );
}

void MainWindow::on_slider_actionTriggered(int action)
{
    deger = ui->slider->value();
    ui->GDtext->setText(QString::number(deger));
}

void MainWindow::on_image_moments_clicked()
{
    rng(12345);
        TreshHold = ui->slider->value();
        Canny( src_gray, canny_output,TreshHold , TreshHold*2, 3 );
        vector<vector<Point> > contours;
        findContours( canny_output, contours, RETR_TREE, CHAIN_APPROX_SIMPLE );
        vector<Moments> mu(contours.size() );
        for( size_t i = 0; i < contours.size(); i++ )
        {
            mu[i] = moments( contours[i] );
        }
        vector<Point2f> mc( contours.size() );
        for( size_t i = 0; i < contours.size(); i++ )
        {
            //add 1e-5 to avoid division by zero
            mc[i] = Point2f( static_cast<float>(mu[i].m10 / (mu[i].m00 + 1e-5)),
                             static_cast<float>(mu[i].m01 / (mu[i].m00 + 1e-5)) );
            //cout << "mc[" << i << "]=" << mc[i] << endl;
        }
        drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
        for( size_t i = 0; i< contours.size(); i++ )
        {
            Scalar color = Scalar( rng.uniform(0, 256),rng.uniform(0, 256),rng.uniform(0, 256));
            drawContours( drawing, contours, (int)i, color, 2 );
            circle( drawing, mc[i], 4, color, -1 );
        }

        im2 = conv.Mat2QImage(drawing);


         pix2 = QPixmap::fromImage(im2);
         sahne2.addPixmap( pix2 );

         ui->graphicsView_2->setScene( &sahne2 );

        cout << "\t Info: Area and Contour Length \n";
        QString text;

        for( size_t i = 0; i < contours.size(); i++ )
        {
            text = text + " * Contour["+ QString::number(i) + "] - Area (M_00) = "+ QString::number(mu[i].m00) +
                         " - Area OpenCV:" + QString::number(contourArea(contours[i])) + " - Length: "+ QString::number(arcLength( contours[i], true ))+"\n";

        }
        ui->textEdit->setText(text);
}
