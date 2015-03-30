/* 
 * File:   StripRemovalPreprocessor.cpp
 * Author: nikolay
 * 
 * Created on March 29, 2015, 12:56 PM
 */

#include <iostream>
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>

#include "StripeRemovalPreprocessor.hpp"

using namespace cv;

StripeRemovalPreprocessor::StripeRemovalPreprocessor(float threshold) : threshold(threshold) {
}


void StripeRemovalPreprocessor::ProcessImage(const Mat& image, Mat& out) {
    int startC = 0, startR = 0, endC = image.cols - 1, endR = image.rows - 1;    
    int sum = 0;

    auto computeMeanMatSum = [](const Mat& mat) {
        float sum = 0;
        std::for_each(mat.begin<Vec3b>(), mat.end<Vec3b>(), [&sum](const Vec3b& pix) {
            sum += pix[0] + pix[1] + pix[2];
        });
        return sum / mat.rows / mat.cols / 3.f;
    };
    
    //We will remove lines in which sum of elements is less than a threshold
    //Looking from top of the image
    while (startC < image.cols && computeMeanMatSum(image.col(startC++)) <= threshold);
    --startC;
    
    //Again, but looking from bottom
    while(endC >= 0 && computeMeanMatSum(image.col(endC--)) <= threshold);
    ++endC;
    
    //left
    while (startR < image.rows && computeMeanMatSum(image.row(startR++)) <= threshold);
    --startR;
    
    //right
    while (endR >= 0 && computeMeanMatSum(image.row(endR--)) <= threshold);
    ++endR;
    
        //No data: return same image
    if(startC > endC || startR > endR) {
        out = image;
        return;
    }
    
    //Cut regions without data
    out = Mat(image, Range(startR, endR), Range(startC, endC));
}