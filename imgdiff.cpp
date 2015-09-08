#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;


std::string mybasename(const std::string& path)
{
    string s = string(path, path.rfind("/") + 1);
    size_t searchresult = s.rfind(".jpeg");
    if(searchresult != string::npos)
    {
        s = string(s, 0, searchresult);
    }
    //cout<<s<<endl;
    return s;
}

namespace brightness
{
Scalar average(const Mat& in)
{
    return mean(in);
}

Mat adjust(const Mat& img, const double factor)
{
    Mat imgproc = Mat(img.cols, img.rows, CV_8UC1);
    img.convertTo(imgproc, CV_8UC1, 1, factor);
    return imgproc;
}

std::pair<Mat, Mat> equalize(Mat&& img1, Mat&& img2)
{
    double b1 = brightness::average(img1)[0];
    double b2 = brightness::average(img2)[0];
    double brghtDiff = b2 - b1;
    cerr<<"brightness difference: "<<brghtDiff<<endl;

    //images differ in brightness
    if(abs(brghtDiff) > 1)
    {
        //shadow clipping and highlight clipping:
        //we actually want the images to be clipped same way
        //to ensure that there's no false motion detection going on
        //assume they are clipped just from one side. Then, the one with
        //average brightness further from mid gray is the one we want to
        //replicate in the other.
        if(abs(127 - b1) > abs(127 - b2))
        {
            //img1 more clipped
            return {move(img1), brightness::adjust(img2, -brghtDiff)};
        }
        else
        {
            //img2 more clipped
            return {brightness::adjust(img1, brghtDiff), move(img2)};
        }
    }
    else
    {
        return make_pair(move(img1), move(img2));
    }
}

} //namespace brightness


Mat toGrayscale(const Mat& img)
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

    auto images = brightness::equalize(move(img1proc), move(img2proc));

    //WTF? img1proc = img1proc2;

    absdiff(img1proc,img2proc,imgdiff);
    threshold(imgdiff, imgdiff2, 40, 255, CV_THRESH_BINARY);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(8,8));
    erode(imgdiff2, imgdiff3, element);

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
    for(size_t i = 0; i < contours.size(); ++i)
    {
        jointContour.insert(jointContour.end(), contours[i].begin(),contours[i].end());
    }

    Rect bounds = boundingRect(jointContour);
    return bounds;
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        cerr<<"Invalid parameters count."<<endl;
        exit(1);
    }

    char* imgpath1 = argv[1];
    char* imgpath2 = argv[2];
    char* cropdir = argv[3];

    Mat img1 = imread(imgpath1);
    Mat img2 = imread(imgpath2);

    Mat imgdiff = compareImages(img1, img2);
    int64_t count = countNonZero(imgdiff);
    //write output: number of pixels different

    if(count > 500)
    {
        auto bounds = motionBounds(imgdiff);
        if((bounds.width >= 0.95* img1.cols) && (bounds.height >= 0.95* img1.rows))
        {
            cerr<<"This looks like an exposure or focus error. Ignoring."<<endl;
            //actually not that different. possibly an exposure error.
            cout<<"0";
            return 0;
        }

        //FIXME: this is dubious and not working anyway
        if((bounds.width > 0.2 * img1.cols) && (bounds.height > 0.2 * img1.rows) && count < 3000 )
        {
            cerr<<"This looks like wind motion. Ignoring."<<endl;
            cout<<"0";
            return 0;
        }

        //cout<<". TL = "<<bounds.tl()<<", BR = "<<bounds.br()<<endl;
        Mat crop1 = img1(bounds);
        Mat crop2 = img2(bounds);
        double aspectratio = (double)crop1.cols / crop1.rows;
        double scalefactor = 0.0;
        size_t targetWidth = 100;
        size_t width = targetWidth;
        size_t targetHeight = 100;
        size_t height = targetHeight;
        if(aspectratio > 1)
        {
            scalefactor = crop1.rows / targetHeight;
            width = targetWidth * aspectratio;
        }
        else
        {
            scalefactor = crop1.cols / targetWidth;
            height = targetHeight / aspectratio;
        }
        Mat crop1s(height, width, CV_32S);
        Mat crop2s(height, width, CV_32S);

        resize(crop1, crop1s, Size(width, height));
        resize(crop2, crop2s, Size(width, height));

        imwrite(string(cropdir) + "/" + mybasename(imgpath1) + "-1.jpeg", crop1);
        imwrite(string(cropdir) + "/" + mybasename(imgpath2) + "-2.jpeg", crop2);

        imwrite(string(cropdir) + "/" + mybasename(imgpath1) + "-1s.jpeg", crop1s);
        imwrite(string(cropdir) + "/" + mybasename(imgpath2) + "-2s.jpeg", crop2s);
        cout<<count<<endl;
    }
    else
    {
        cout<<"0";
    }

    return 0;
}
