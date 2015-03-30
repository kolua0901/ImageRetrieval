/* 
 * File:   FeatureExtractor.hpp
 * Author: nikolay
 *
 * Created on March 28, 2015, 4:06 PM
 */

#ifndef FEATUREEXTRACTOR_HPP
#define	FEATUREEXTRACTOR_HPP

#include <opencv2/core/core.hpp>

using namespace cv;

//Abstract class for feature extraction
class FeatureExtractor {
public:
    virtual ~FeatureExtractor() {}; 
    //Provided method
    virtual void ExtractFeatures(const Mat& in, Mat& descriptors) = 0;
};

#endif	/* FEATUREEXTRACTOR_HPP */

