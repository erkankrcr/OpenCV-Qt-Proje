#ifndef OPENCVQTCONVERTER_H
#define OPENCVQTCONVERTER_H


#include <QImage>
#include <opencv2/opencv.hpp>



class OpencvQtConverter
{
public:
    OpencvQtConverter();

    QImage Mat2QImage(cv::Mat const& src);
    cv::Mat QImage2Mat(QImage const& src);

};

#endif // OPENCVQTCONVERTER_H
