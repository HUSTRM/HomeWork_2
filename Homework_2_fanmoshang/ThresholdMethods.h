//
// Created by top on 18-11-26.
//



#include <opencv2/core/mat.hpp>

#ifndef IMAGESEGMENTATION_THRESHOLDMETHODS_H
#define IMAGESEGMENTATION_THRESHOLDMETHODS_H

#endif //IMAGESEGMENTATION_THRESHOLDMETHODS_H

void threshold_myself(int threshold);
void auto_set_threshold();
int otsuMethod(cv::Mat src);
void my_localAdaptiveThresholding();
void src_adaptiveThreshold();
void blockedThresholdSegmentation();
void averMovingSegmentation();
std::vector<int> MultiThreshold(cv::Mat src);