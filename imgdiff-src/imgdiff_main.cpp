/*
 * File:   imgdiff_main.cpp
 * Author: Jarek Dziedzic
 *
 * Created on 16 January 2016
 */

#include "imgdiff.h"
#include "brightness.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

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


// TODO: The inputs should be: images, sensitivity
// TODO: Outputs: number of pixels and the motion bounds
// TODO: Some external tool should then interpret the bounds and make necessary
// TODO: crops. Maybe.
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

    auto absBriDiff = abs(brightness::difference(img1, img2));
    if (absBriDiff > 70 )
    {
        //don't bother comparing. It's a massively different image.
        cerr<<"These images vary very much in exposure. Ignoring. (brightness diff = "<< absBriDiff<<")"<<endl;
        cout<<0<<endl;
        return 0;
    }
    Mat imgdiff = compareImages(img1, img2);
    int64_t count = countNonZero(imgdiff);
    if (count > 480)
    {
        auto bounds = motionBounds(imgdiff);
        //cout<<". TL = "<<bounds.tl()<<", BR = "<<bounds.br()<<endl;
        Mat crop1 = img1(bounds);
        Mat crop2 = img2(bounds);
        double aspectratio = (double)crop1.cols / crop1.rows;
        double scalefactor = 0.0;
        size_t targetWidth = 100;
        size_t width = targetWidth;
        size_t targetHeight = 100;
        size_t height = targetHeight;

        if ((bounds.height < 0.15 * img1.rows) && (count < 10000)  && (bounds.y < img1.rows*0.05))
        {
            cerr<<"This looks like light bleed. Ignoring."<<endl;
            cout<<"0";
            return 0;
        }

        if (aspectratio > 1)
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

        //write output: number of pixels different between the images
        cout<<count<<endl;
    }
    else
    {
        cout<<"0";
    }

    return 0;
}
