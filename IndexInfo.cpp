/* 
 * File:   IndexInfo.cpp
 * Author: nikolay
 * 
 * Created on March 29, 2015, 1:04 AM
 */

#include "IndexInfo.hpp"

int IndexInfo::GetNumElements() {
    return fileNames.size();
}

const std::vector<std::string>& IndexInfo::GetFileNames() {
    return fileNames;
}

const std::vector<int>& IndexInfo::GetNumFeatures() {
    return numFeatures;
}

void IndexInfo::Add(std::string name, int numFeature) {
    fileNames.push_back(name);
    numFeatures.push_back(numFeature);
}
