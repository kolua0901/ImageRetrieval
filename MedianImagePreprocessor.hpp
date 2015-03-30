/* 
 * File:   MedianImagePreprocessor.hpp
 * Author: nikolay
 *
 * Created on March 29, 2015, 12:31 PM
 */

#ifndef MEDIANIMAGEPREPROCESSOR_HPP
#define	MEDIANIMAGEPREPROCESSOR_HPP

#include "ImagePreprocessor.hpp"

//Image preprocessor that uses median filter
class MedianImagePreprocessor : public ImagePreprocessor {
public:
    MedianImagePreprocessor(int size = 3);
    
    void ProcessImage(const cv::Mat& image, cv::Mat& out);
private:
    int size;
};

#endif	/* MEDIANIMAGEPREPROCESSOR_HPP */

