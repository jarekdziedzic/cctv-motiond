/* 
 * File:   brightness.h
 * Author: Jarek Dziedzic
 *
 * Created on 16 January 2016
 */

#ifndef BRIGHTNESS_H
#define BRIGHTNESS_H

#include <opencv2/core/core.hpp>

namespace brightness {
    
cv::Scalar average(const cv::Mat& in);

double difference(const cv::Mat& img1, const cv::Mat& img2);

} //namespace brightness

#endif /* BRIGHTNESS_H */

