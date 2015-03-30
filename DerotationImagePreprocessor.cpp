/* 
 * File:   UnrotationImagePreprocessor.cpp
 * Author: nikolay
 * 
 * Created on March 29, 2015, 5:31 PM
 */

#include "DerotationImagePreprocessor.hpp"

#include <vector>
#include <algorithm>
#include <iostream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

void DerotationImagePreprocessor::ProcessImage(const Mat& image, Mat& out) {
    std::vector<int> startColData(image.rows), endColData(image.rows);
        
    int start = 0, end = image.cols - 1;
    //For each row
    
    auto sumFunc = [](const Vec3b& v) {
        return v[0] + v[1] + v[2];
    };
    
    for (int i = 0; i < image.rows; ++i) {
        start = 0;
        end = image.cols - 1;
        //Seeking for index there data starts...
        while(start < image.cols && !sumFunc(image.at<Vec3b>(i, start++)));
        startColData[i] = start - 1;
        
        //and ends
        while(end >= 0 && !sumFunc(image.at<Vec3b>(i, end--)));
        endColData[i] = end + 1;
    }    
    
    //Computing total number of zero pixels (out pixels)
    int numOutPixels = 0;
    for (int i : startColData)
        numOutPixels += i;
    for (int i : endColData)
        numOutPixels += image.cols - i;
    
    //Too few out pixels: no rotation.
    if (numOutPixels < image.cols * image.rows * 0.1f) {
        out = image;
        return;
    }
    std::cout << "Rotation found" << std::endl;
    
    //Seeking for index with peak. It is a index there the second line starts
    int startSecLine = std::distance(startColData.begin(), 
            std::min_element(startColData.begin(), startColData.end())),
        endSecLine = std::distance(endColData.begin(),
            std::max_element(endColData.begin(), endColData.end()));
    
    //Solve least squares problem
    std::vector<float> coefs = GetLSLData(startColData, endColData, startSecLine, endSecLine);
    
    //Computing a direction of rotation (cw or ccw) 
    //Works in assumption that real image has landscape orientation!
    float lengthFirst = sqrt(sqr(endColData[0] - endColData[endSecLine]) + sqr(endSecLine - 1)),
          lengthSecond = sqrt(sqr(endColData[endColData.size()-1] - endColData[endSecLine]) + sqr(endColData.size() - endSecLine));
    bool ccwRotation = lengthFirst > lengthSecond;
    
    //Compute angles (in degrees) from obtained coefficients
    std::vector<float> angles(coefs.size());
    for(int i = 0; i < coefs.size(); ++i) {
        if (ccwRotation) 
            angles[i] = acos(sgn(coefs[i]) / sqrt(sqr(coefs[i]) + 1)) * 180.f / M_PI;
        else 
            angles[i] = acos(coefs[i] / sqrt(sqr(coefs[i]) + 1)) * 180.f / M_PI;            
        
        //These indices contain a angle that more than real angle on 90 degrees  
        if (i == 0 || i == 3)
            angles[i] = angles[i] - 90.f;
    }
    
    //Total angle is mean of 4 estimated angles
    float totalAngle = 0;
    for(float a : angles) {
        totalAngle += a;
    }
    totalAngle /= 4;
    
    //For cw rotation needed with negative angle sign
    totalAngle = ccwRotation ? totalAngle : -totalAngle;
    
    //Rotate image on total angle
    RotateImage(image, out, totalAngle);
}

//Least Squares problem for lines in which data starts and ends.
std::vector<float> DerotationImagePreprocessor::GetLSLData(const std::vector<int>& start, 
        const std::vector<int>& end, int secondStart, int secondEnd) {
    std::vector<float> ret = GetLSLData(start, secondStart);
    std::vector<float> endLineCoefs = GetLSLData(end, secondEnd);
    ret.insert(ret.end(), endLineCoefs.begin(), endLineCoefs.end());
    
    return ret;
}

//Least Squares problem for 2 lines where secondLineIndex points there the second line starts
std::vector<float> DerotationImagePreprocessor::GetLSLData(const std::vector<int>& data, int secondLineIndex) {
    std::vector<float> ret(2);
    std::vector<std::pair<int, int> > toProcess;
    for (int i = 0; i < secondLineIndex; ++i) {
        toProcess.push_back(std::pair<int, int>(i, data[i]));
    }
    ret[0] = LeastSquaresLine(toProcess).first;
    toProcess.clear();
    
    for (int i = secondLineIndex; i < data.size(); ++i) {
        toProcess.push_back(std::pair<int, int>(i, data[i]));
    }
    ret[1] = LeastSquaresLine(toProcess).first;
    
    return ret;
}

//Least Squares problem for 2D line case
//Returns coefficients of fitting line
std::pair<float, float> DerotationImagePreprocessor::LeastSquaresLine(const std::vector<std::pair<int, int> >& data) {
    float meanX, meanY = 0;
    for(std::pair<int, int> p : data) {
        meanX += p.second;
        meanY += p.first;
    }
    meanX /= data.size();
    meanY /= data.size();
    
    float corrXY = 0, dispX = 0;
    for(std::pair<int, int> p : data) {
        dispX += (p.second - meanX) * (p.second - meanX);
        corrXY += (p.first - meanY) * (p.second - meanX);
    }
    float slope = corrXY / dispX, yIntercept = meanY - slope * meanX;
    return std::pair<float, float>(slope, yIntercept);
}

void DerotationImagePreprocessor::RotateImage(const Mat& image, Mat& out, float angle) {
    Point2f center(image.cols / 2.f, image.rows / 2.f);
    Mat rotMat = getRotationMatrix2D(center, angle, 1.f);
    warpAffine(image, out, rotMat, image.size());
}
