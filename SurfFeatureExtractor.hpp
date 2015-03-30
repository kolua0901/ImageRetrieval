/* 
 * File:   SurfFeatureExtractor.hpp
 * Author: nikolay
 *
 * Created on March 28, 2015, 4:21 PM
 */

#ifndef SURFFEATUREEXTRACTOR_HPP
#define	SURFFEATUREEXTRACTOR_HPP

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include "FeatureExtractor.hpp"

using namespace cv;

class SurfFeatureExtractor : virtual public FeatureExtractor {
public:
    SurfFeatureExtractor(int minHessian = 100);
    void ExtractFeatures(const Mat& in, Mat& descriptors);
private:
    SurfFeatureDetector detector;
    SurfDescriptorExtractor extractor;
    
    int minHessian;
};

#endif	/* SURFFEATUREEXTRACTOR_HPP */

