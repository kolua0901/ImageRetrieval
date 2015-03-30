/* 
 * File:   StripRemovalPreprocessor.hpp
 * Author: nikolay
 *
 * Created on March 29, 2015, 12:56 PM
 */

#ifndef STRIPREMOVALPREPROCESSOR_HPP
#define	STRIPREMOVALPREPROCESSOR_HPP

#include <opencv2/core/core.hpp>

#include "ImagePreprocessor.hpp"

//Horizontal and vertical stripes removal preprocessor 
class StripeRemovalPreprocessor : public ImagePreprocessor{
public:
    StripeRemovalPreprocessor(float threshold = 10.f);
    void ProcessImage(const cv::Mat& image, cv::Mat& out);
private:
    const float threshold;         //Threshold for black line detection
};

#endif	/* STRIPREMOVALPREPROCESSOR_HPP */

