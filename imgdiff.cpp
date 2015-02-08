#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <string>
//#include <boost/bind.hpp>

using namespace std;
using namespace cv;
//using namespace boost;

enum ReturnType {
    IMAGES_SIMILAR = 0,
    IMAGES_DIFFERENT = 66,
};

std::string mybasename(const std::string& path)
{
    string s = string(path, path.rfind("/") + 1);
    size_t searchresult = s.rfind(".jpeg");
    if(searchresult != string::npos)
    {
        s = string(s, 0, searchresult);
    }
    cout<<s<<endl;
    return s;
}

Scalar getAverageBrightness(const Mat& in)
{
    return mean(in);
}

Mat equalizeBrightness(Mat& img1, Mat& img2)
{
    Scalar b1 = getAverageBrightness(img1);
    Scalar b2 = getAverageBrightness(img2);
    Mat img1proc = Mat(img1.cols, img1.rows, CV_8UC1);

    double brghtDiff = (double)(b2 - b1)[0];
    cout<<"brightness difference: "<<brghtDiff<<endl;
    img1.convertTo(img1proc, CV_8UC1, 1, brghtDiff);
    return img1proc;
}

int main(int argc, char** argv)
{
    if(argc != 4)
    {
        cout<<"Invalid parameters count."<<endl;
        exit(1);
    }

    char* imgpath1 = argv[1];
    char* imgpath2 = argv[2];
    char* cropdir = argv[3];

    Mat img1 = imread(imgpath1);
    Mat img2 = imread(imgpath2);

    Mat img1proc = Mat(img1.cols, img1.rows, CV_8UC1);
    Mat img2proc = Mat(img2.cols, img2.rows, CV_8UC1);
    Mat imgdiff = Mat(img2.cols, img2.rows, CV_8UC1);
    Mat imgdiff2 = Mat(img2.cols, img2.rows, CV_8UC1);
    Mat imgdiff3 = Mat(img2.cols, img2.rows, CV_8UC1);
    cvtColor(img1, img1proc, CV_RGB2GRAY);
    cvtColor(img2, img2proc, CV_RGB2GRAY);
    Mat img1proc2 = equalizeBrightness(img1proc, img2proc);

    img1proc = img1proc2;
    absdiff(img1proc,img2proc,imgdiff);
    threshold(imgdiff, imgdiff2, 40, 255, CV_THRESH_BINARY);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(8,8));
    erode(imgdiff2, imgdiff3, element);

    //too slow :(
    //cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(40,40));
    //dilate(imgdiff, imgdiff, element);
    //cout<<countNonZero(imgdiff)<<endl;

    int64_t count = countNonZero(imgdiff3);
    cout<<"Different pixels count: "<<count<<endl;
    if(count > 500)
    {
        //different!
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        vector<Point> jointContour;

        findContours( imgdiff3, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        for(size_t i = 0; i < contours.size(); ++i)
        {
            jointContour.insert(jointContour.end(), contours[i].begin(),contours[i].end());    
        }
       
        Rect bounds = boundingRect(jointContour); 

        if((bounds.width >= 0.95* img1.cols) && (bounds.height >= 0.95* img1.rows))
        {
            cout<<"This looks like an exposure or focus error. Ignoring."<<endl;
            //actually not that different. possibly an exposure error.
            return IMAGES_SIMILAR;
        }

        if((bounds.width > 0.2 * img1.cols) && (bounds.height > 0.2 * img1.rows) && count < 3000 )
        {
            cout<<"This looks like wind motion. Ignoring."<<endl;
            return IMAGES_SIMILAR;
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
        
        return IMAGES_DIFFERENT;
    }

    return IMAGES_SIMILAR;
}
