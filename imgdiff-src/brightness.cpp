/*
 * File:   brightness.cpp
 * Author: Jarek Dziedzic
 *
 * Created on 16 January 2016
 */

#include "brightness.h"

using namespace std;
using namespace cv;

namespace brightness {

Scalar average(const Mat& in)
{
    return mean(in);
}

double difference(const Mat& img1, const Mat& img2)
{
    double b1 = brightness::average(img1)[0];
    double b2 = brightness::average(img2)[0];
    double brghtDiff = b2 - b1;
    return brghtDiff;
}

} //namespace brightness
