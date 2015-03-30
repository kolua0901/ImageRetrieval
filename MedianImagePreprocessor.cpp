/* 
 * File:   MedianImagePreprocessor.cpp
 * Author: nikolay
 * 
 * Created on March 29, 2015, 12:31 PM
 */

#include "MedianImagePreprocessor.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

MedianImagePreprocessor::MedianImagePreprocessor(int size) : size(size) {
}

void MedianImagePreprocessor::ProcessImage(const Mat& image, Mat& out) {
    out = Mat(image);
    medianBlur(image, out, size);
}