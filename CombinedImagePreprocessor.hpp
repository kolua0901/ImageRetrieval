/* 
 * File:   CombinedImagePreprocessor.hpp
 * Author: nikolay
 *
 * Created on March 29, 2015, 1:50 PM
 */

#ifndef COMBINEDIMAGEPREPROCESSOR_HPP
#define	COMBINEDIMAGEPREPROCESSOR_HPP

#include "ImagePreprocessor.hpp"
#include "MedianImagePreprocessor.hpp"
#include "StripeRemovalPreprocessor.hpp"
#include "DerotationImagePreprocessor.hpp"

//Combination of median, stripes removal and unrotation preprocessors
class CombinedImagePreprocessor : public ImagePreprocessor {
public:
    CombinedImagePreprocessor(int size = 3);
    void ProcessImage(const cv::Mat& image, cv::Mat& out);
private:
    MedianImagePreprocessor mip;
    StripeRemovalPreprocessor srp;
    DerotationImagePreprocessor uip;
};

#endif	/* COMBINEDIMAGEPREPROCESSOR_HPP */

