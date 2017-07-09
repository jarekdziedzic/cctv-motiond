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

/**
 * @brief Equalizes the exposure of two images.
 *
 * Aims to achieve equally bad clipping in both images.
 *
 * @return pair of {img1, img2}
 */
std::pair<cv::Mat, cv::Mat> equalize(cv::Mat&& img1, cv::Mat&& img2);

} //namespace exposure


#endif /* EXPOSURE_H */

