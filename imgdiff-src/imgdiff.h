/* 
 * File:   imgdiff.h
 * Author: Jarek Dziedzic
 *
 * Created on 16 January 2016
 */

#ifndef IMGDIFF_H
#define IMGDIFF_H

#include <opencv2/core/core.hpp>

cv::Mat compareImages(cv::Mat& img1, cv::Mat& img2);

/**
 * @brief Given a imgdiff returns a bounding rect of non zero value pixels
 */
cv::Rect motionBounds(const cv::Mat& img);

#endif /* IMGDIFF_H */

