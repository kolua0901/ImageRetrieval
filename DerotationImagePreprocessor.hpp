/* 
 * File:   UnrotationImagePreprocessor.hpp
 * Author: nikolay
 *
 * Created on March 29, 2015, 5:31 PM
 */

#ifndef UNROTATIONIMAGEPREPROCESSOR_HPP
#define	UNROTATIONIMAGEPREPROCESSOR_HPP

#include "ImagePreprocessor.hpp"

#include <opencv2/core/core.hpp>

//Class for image derotation
class DerotationImagePreprocessor : public ImagePreprocessor {
public:
    typedef std::pair<float, float> ffp;
    
    void ProcessImage(const cv::Mat& image, cv::Mat& out);
private:
    std::pair<float, float> LeastSquaresLine(const std::vector<std::pair<int, int> > &data);
    std::vector<float> GetLSLData(const std::vector<int> &start, const std::vector<int> &end,
            int secondStart, int secondEnd);
    std::vector<float> GetLSLData(const std::vector<int> &data, int secondLineIndex);
    void RotateImage(const cv::Mat& image, cv::Mat& out, float angle);
    
    template<typename T>
    T sqr(const T& x) { return x * x; }
    template <typename T> int sgn(T val) { return (T(0) < val) - (val < T(0)); }
};

#endif	/* UNROTATIONIMAGEPREPROCESSOR_HPP */

