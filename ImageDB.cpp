/* 
 * File:   ImageDB.cpp
 * Author: nikolay
 * 
 * Created on March 28, 2015, 5:28 PM
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

#include <boost/filesystem.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/flann/flann.hpp>

#include "ImageDB.hpp"
#include "CombinedImagePreprocessor.hpp"

using namespace cv;
using namespace boost::filesystem;

const std::string ImageDB::indexFilename = "index.bin";
const std::string ImageDB::descriptorsFilename = "descriptors.xml";
const std::string ImageDB::descriprorsMatName = "descriptors";
const std::string ImageDB::infoFilename = "name_index.txt";

ImageDB::ImageDB(const std::string& folder, bool forceBuild) : 
        folder(folder), forceBuild(forceBuild) {
    featureExtractor = new SurfFeatureExtractor();
    try {
        Load();
    }
    catch (...)
    {
        delete featureExtractor;
        throw;
    }
    imagePreprocessor = new CombinedImagePreprocessor(medFiltSize);
}

ImageDB::~ImageDB() {
    delete featureExtractor;
    delete imagePreprocessor;
    if (index != NULL)
        index->release();
    delete index;
}

typedef directory_iterator dir_iter;

void ImageDB::Load() {
    path p(folder);
    if (exists(p) && is_directory(p)) {
        dir_iter end;
        
        //Searching for existing saved index if there is no force build command. 
        //If found read it and return
        if (!forceBuild && FindAndReadIndex(p))
            return;
        
        //Index not found or not loaded correctly, so loading images and computing descriptors
        for(dir_iter iter(p); iter != end; ++iter) {
            if (is_regular_file(*iter))
            {
                //Loading file
                std::string filename = iter->path().string();
                std::cout << "Reading file: " << filename << std::endl;
                Mat image = imread(filename, CV_LOAD_IMAGE_COLOR);
                if (image.data == NULL) {
                    std::cout << "Not a image. Continuing" << std::endl;
                    continue;
                }
                //Computing features
                Mat dsc;
                featureExtractor->ExtractFeatures(image, dsc);                
                descriptors.push_back(dsc);
                //Adding info about filename and number of obtained features
                info.Add(filename, dsc.rows);
            }
        }
    }
    //Nothing to do here
    else {
        throw "Bad folder!";
    }
    
    //Building index
    BuildIndex();
    //Saving index with descriptors
    WriteIndex();
}


bool ImageDB::FindAndReadIndex(path p) {
    dir_iter end;
    dir_iter iterIndex(p), iterDescriptors(p), iterInfo(p);
        
    //Looking for existing index
    std::find_if(iterIndex, end, [this](const directory_entry& e) {
        return indexFilename == e.path().filename().string();
    });
    std::find_if(iterDescriptors, end, [this](const directory_entry& e) {
        return descriptorsFilename == e.path().filename().string();
    });
    std::find_if(iterInfo, end, [this](const directory_entry& e) {
        return infoFilename == e.path().filename().string();
    });    
    
    if (iterIndex == end || iterDescriptors == end || iterInfo == end)
        return false;
    
   //Index found 
    std::cout << "Index found. Trying to load" << std::endl; 
    //trying to load index
    try {
        ReadIndex(iterIndex, iterDescriptors, iterInfo);
        std::cout << "Index was successfully loaded" << std::endl;
        return true;
    }
    catch (...) {
        std::cout << "Failed to load index. Reading files and building index" << std::endl;
        return false;
    }
}

//Read index, descriptors and indexInfo
void ImageDB::ReadIndex(const dir_iter& iterIndex, const dir_iter& iterDescriptors, const dir_iter& iterInfo) {
    std::ifstream ifs(iterInfo->path().string());
    boost::archive::text_iarchive ia(ifs);
    ia >> info;
    
    FileStorage fs(iterDescriptors->path().string(), FileStorage::READ);
    fs[descriprorsMatName] >> descriptors;
                
    flann::SavedIndexParams sip(iterIndex->path().string().c_str());
    index = new flann::Index(descriptors, sip);
}

//Save index, descriptors and indexInfo
void ImageDB::WriteIndex() {
    std::cout << "Saving index" << std::endl;

    std::string fullIndexFilename = folder + '/' + indexFilename;
    std::string fullDesFilname = folder + '/' + descriptorsFilename;
    std::string fullInfoFilename = folder + '/' + infoFilename;
    if (index != NULL) {
        index->save(fullIndexFilename);
        
        FileStorage file(fullDesFilname, FileStorage::WRITE);
        file << descriprorsMatName << descriptors;
        file.release();
        
        std::ofstream ofs(fullInfoFilename);
        boost::archive::text_oarchive oa(ofs);
        oa << info;
    }
}

void ImageDB::BuildIndex() {
    std::cout << "Building index" << std::endl;
    
    flann::KDTreeIndexParams params(kdParam);
    index = new flann::Index(descriptors, params);
}

typedef std::pair<std::string, float> sfpair;

//Searching for similar images from index
//Returns vector with pair (filename, rank)
std::vector<sfpair> ImageDB::Search(const Mat& queryImage) {    
    //Preprocessing image
    Mat preprocessedImage;
    imagePreprocessor->ProcessImage(queryImage, preprocessedImage);
    
//    imshow("pr", preprocessedImage);
//    waitKey(0);
    
    //Extracting features
    Mat queryFeatures;
    featureExtractor->ExtractFeatures(preprocessedImage, queryFeatures);
    
    Mat indices(queryFeatures.rows, k, CV_32S);
    Mat dists(queryFeatures.rows, k, CV_32F);
    
    //Search for similar descriptors using KD-tree
    std::cout << "Searching" << std::endl;
    index->knnSearch(queryFeatures, indices, dists, k);
    std::cout << "Computing rank" << std::endl;
    ConvertFeatureOccurenceToIndex(indices);
    return ComputeSimilarityRank(indices, dists);
}

//Convert features indices to image indices
void ImageDB::ConvertFeatureOccurenceToIndex(Mat& mat) {
    auto numFeatureOccurences = info.GetNumFeatures();
    for(auto it = mat.begin<int>(); it != mat.end<int>(); ++it) {
        int v = 0, k = 0;
        while (v < *it) {
            v += numFeatureOccurences[k++];
        }
        *it = k - 1;
    }
}

//Compute similarity rank from indices and distance matrices. 
//Returns vector with pair (filename, rank)
std::vector<sfpair> ImageDB::ComputeSimilarityRank(const Mat& indices, const Mat& dists) {
    std::vector<float> votes(info.GetNumElements());
    for (int i = 0; i < indices.rows; ++i) {
        for (int j = 0; j < indices.cols; ++j) {
            //Votes are proportionally to Gaussian function value of distance between descriptors
            votes[indices.at<int>(i, j)] += exp(-dists.at<float>(i, j) * dists.at<float>(i, j));
        }
    }
    
    auto fileNames = info.GetFileNames();
    std::vector<sfpair> ret;
    for (int i = 0; i < info.GetNumElements(); ++i) {
        ret.push_back(sfpair(fileNames[i], votes[i]));
    }
    //Sorting results by rank descending
    std::sort(ret.begin(), ret.end(), [](const sfpair& l, const sfpair& r) {
        return l.second > r.second;
    });
    
    return ret;
}