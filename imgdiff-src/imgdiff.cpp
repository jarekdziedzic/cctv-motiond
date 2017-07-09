#include "brightness.h"
#include "exposure.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
using namespace cv;


static Mat toGrayscale(const Mat& img)
{
    Mat imgproc = Mat(img.cols, img.rows, CV_8UC1);
    cvtColor(img, imgproc, CV_RGB2GRAY);
    return imgproc;
}

Mat compareImages(Mat& img1, Mat& img2)
{
    Mat img1proc = toGrayscale(img1);
    Mat img2proc = toGrayscale(img2);
    Mat imgdiff = Mat(img2.cols, img2.rows, CV_8UC1);
    Mat imgdiff2 = Mat(img2.cols, img2.rows, CV_8UC1);
    Mat imgdiff3 = Mat(img2.cols, img2.rows, CV_8UC1);

    auto images = exposure::equalize(move(img1proc), move(img2proc));

    absdiff(images.first,images.second, imgdiff);
    threshold(imgdiff, imgdiff2, 40, 255, CV_THRESH_BINARY);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(8,8));
    erode(imgdiff2, imgdiff3, element);
//    imwrite("/tmp/a1.jpeg", images.first);
//
//    imwrite("/tmp/a2.jpeg", images.second);
//
//    imwrite("/tmp/ad.jpeg", imgdiff3);

    //too slow :(
    //cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(40,40));
    //dilate(imgdiff, imgdiff, element);
    //cout<<countNonZero(imgdiff)<<endl;
    return imgdiff3;
}

/**
 * @brief Given a imgdiff returns a bounding rect of non zero value pixels
 */
Rect motionBounds(const Mat& img)
{
    //different!
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    vector<Point> jointContour;

    findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    for (size_t i = 0; i < contours.size(); ++i)
    {
        jointContour.insert(jointContour.end(), contours[i].begin(),contours[i].end());
    }

    Rect bounds = boundingRect(jointContour);
    return bounds;
}

