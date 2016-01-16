/* 
 * File:   exposure.h
 * Author: Jarek Dziedzic
 *
 * Created on 16 January 2016
 */

#ifndef EXPOSURE_H
#define EXPOSURE_H

#include <opencv2/core/core.hpp>

namespace exposure {

/**
 * @brief Increase/decrease exposure by a specified factor.
 * This function returns a new adjusted image.
 */
cv::Mat adjust(const cv::Mat& img, const double factor);

std::pair<cv::Mat, cv::Mat> equalize(cv::Mat&& img1, cv::Mat&& img2);

} //namespace exposure


#endif /* EXPOSURE_H */

