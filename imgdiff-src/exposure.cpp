/*
 * File:   exposure.cpp
 * Author: Jarek Dziedzic
 *
 * Created on 16 January 2016
 */

#include "exposure.h"
#include "brightness.h"
#include <iostream>

using namespace cv;
using namespace std;

namespace exposure {
Mat adjust(const Mat& img, const double factor)
{
    Mat imgproc = img;
    cerr<<"exposure adjustment factor = "<<factor<<endl;
    auto multiplier = pow(2, factor);
    auto f =  [multiplier](unsigned char elem)
        {
            //white-clip the result
            return min(255, (int)(elem * multiplier));
        };
    std::transform(img.datastart, img.dataend, imgproc.data, f);
    return imgproc;
}

std::pair<Mat, Mat> equalize(Mat&& img1, Mat&& img2)
{
    double b1 = brightness::average(img1)[0];
    double b2 = brightness::average(img2)[0];
    double brghtDiff = b2 - b1;
    cerr<<"b1 = "<<b1<<", b2 = "<<b2<<endl;

    cerr<<"brightness difference: "<<brghtDiff<<endl;

    //images differ in brightness
    if (abs(brghtDiff) > 1)
    {
        //shadow clipping and highlight clipping:
        //we actually want the images to be clipped same way
        //to ensure that there's no false motion detection going on
        //assume they are clipped just from one side. Then, the one with
        //average brightness further from mid gray is the one we want to
        //replicate in the other.
        const int midGray = 127;

        if (abs(b1 - midGray) > abs(b2 - midGray))
        {
            //img1 more clipped
            return {move(img1), exposure::adjust(img2, log2(b1/b2))};
        }
        else
        {
            //img2 more clipped
            return {exposure::adjust(img1, log2(b2/b1)), move(img2)};
        }
    }
    else
    {
        return make_pair(move(img1), move(img2));
    }
}

} //namespace exposure
