/* 
 * File:   SurfFeatureExtractor.cpp
 * Author: nikolay
 * 
 * Created on March 28, 2015, 4:22 PM
 */

#include "SurfFeatureExtractor.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <vector>

SurfFeatureExtractor::SurfFeatureExtractor(int minHessian) : minHessian(minHessian), detector(minHessian) {   
}

void SurfFeatureExtractor::ExtractFeatures(const Mat& in, Mat& descriptors) {
    std::vector<KeyPoint> keypoints;
    
    detector.detect(in, keypoints);
    extractor.compute(in, keypoints, descriptors);
}
