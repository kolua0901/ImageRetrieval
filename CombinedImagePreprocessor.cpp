/* 
 * File:   CombinedImagePreprocessor.cpp
 * Author: nikolay
 * 
 * Created on March 29, 2015, 1:50 PM
 */

#include "CombinedImagePreprocessor.hpp"

#include <opencv2/highgui/highgui.hpp>

CombinedImagePreprocessor::CombinedImagePreprocessor(int size) : mip(size) {
}

void CombinedImagePreprocessor::ProcessImage(const cv::Mat& image, cv::Mat& out) {
    cv::Mat tmp1, tmp2;
    mip.ProcessImage(image, tmp1);
    uip.ProcessImage(tmp1, tmp2);
    srp.ProcessImage(tmp2, out);
    
//    cv::imshow("Preprocessed image", out);
//    cv::waitKey(0);
}