/* 
 * File:   IndexInfo.hpp
 * Author: nikolay
 *
 * Created on March 29, 2015, 1:04 AM
 */

#ifndef INDEXINFO_HPP
#define	INDEXINFO_HPP

#include <vector>
#include <string>

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/vector.hpp>

//Index info class. Contains filenames of images and number of calculated features
//for each (needed for conversion from descriptor index to image index))
//Serializable class (boost::serialization is used)
class IndexInfo {
public:
    int GetNumElements();
    void Add(std::string name, int numFeatures);
    
    const std::vector<std::string>& GetFileNames ();
    const std::vector<int>& GetNumFeatures ();
private:
    friend class boost::serialization::access;
    
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & fileNames;
        ar & numFeatures;
    }
    
    std::vector<std::string> fileNames;
    std::vector<int> numFeatures;
};

#endif	/* INDEXINFO_HPP */

