/* 
 * File:   ImagePreprocessor.hpp
 * Author: nikolay
 *
 * Created on March 29, 2015, 12:27 PM
 */

#ifndef IMAGEPREPROCESSOR_HPP
#define	IMAGEPREPROCESSOR_HPP

#include <opencv2/core/core.hpp>

//Abstract class for image preprocessing
class ImagePreprocessor {
public:
    virtual ~ImagePreprocessor() {};
    //Provided method
    virtual void ProcessImage(const cv::Mat& image, cv::Mat& out) = 0;
};

#endif	/* IMAGEPREPROCESSOR_HPP */