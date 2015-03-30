/* 
 * File:   ImageDB.hpp
 * Author: nikolay
 *
 * Created on March 28, 2015, 5:28 PM
 */

#ifndef IMAGEDB_HPP
#define	IMAGEDB_HPP

#include <string>
#include <vector>

#include <boost/filesystem.hpp>

#include <opencv2/flann/flann.hpp>

#include "FeatureExtractor.hpp"
#include "SurfFeatureExtractor.hpp"
#include "ImagePreprocessor.hpp"
#include "IndexInfo.hpp"

using namespace cv;

//Main class for image retrieval
class ImageDB {
public:
    typedef std::pair<std::string, float> sfpair;
    typedef boost::filesystem::directory_iterator dir_iter;
    
    ImageDB(const std::string& folder, bool forceBuild = false);
    ~ImageDB();
    
    std::vector<sfpair> Search(const Mat& queryImage);
private:
    static const std::string indexFilename;         //Some filename constants
    static const std::string descriptorsFilename;
    static const std::string descriprorsMatName;
    static const std::string infoFilename;
    
    std::string folder;                             //Folder name
    bool forceBuild;                                //Build index even if there is an existing saved index
    
    static const int k = 3;                         //Number of nearest neighbors
    static const int kdParam = 5;                   //KD-tree parameter
    static const int medFiltSize = 5;               //Median filter size

    IndexInfo info;                                 //Some needed info about index
    Mat descriptors;                                //Descriptors for building index
    FeatureExtractor *featureExtractor;             //Feature extractor class
    ImagePreprocessor *imagePreprocessor;           //Query image preprocessor
    cv::flann::Index *index;                        //Index
    
    void Load();
    
    void BuildIndex();
    bool FindAndReadIndex(boost::filesystem::path p);
    void ReadIndex(const dir_iter& iterIndex, const dir_iter& iterDescriptors, const dir_iter& iterInfo);
    void WriteIndex();
    
    void ConvertFeatureOccurenceToIndex(Mat& mat);
    std::vector<sfpair> ComputeSimilarityRank(const Mat& indices, const Mat& dists);
};

#endif	/* IMAGEDB_HPP */

