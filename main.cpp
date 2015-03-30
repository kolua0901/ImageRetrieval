#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/core/core.hpp>

#include <cstdlib>

#include "FeatureExtractor.hpp"
#include "SurfFeatureExtractor.hpp"
#include "ImageDB.hpp"
using namespace cv;

int main(int argc, char* argv[])
{
    if (argc != 3 && argc != 4) {
        std::cout << "Wrong parameters. Usage: index_folder query_image force_buld (optional, 0 by default)" << std::endl;
        return 1;
    }
    bool forceBuild = false;
    if (argc == 4) 
        forceBuild = atoi(argv[3]) != 0;
    
    try {
        //Creating image index
        ImageDB db(argv[1], forceBuild);
    
        //Reading image and searching for similar to it
        Mat img = imread(argv[2], CV_LOAD_IMAGE_COLOR);
        std::vector<ImageDB::sfpair> res = db.Search(img);
    
        //Showing results
        std::cout << "Images similar to " << argv[2] << ':' << std::endl;
        float maxRank = res[0].second;
        int i = 0;
        for(int i = 0; i < res.size(); ++i) {
            //Showing result for feature which rank is more than 0.2 * maxRank
            if (!(res[i].second > maxRank * 0.2f))
                break;
            std::cout << res[i].first << ' ' << res[i].second << std::endl;
        }
    }
    catch (const char* msg) {
        std::cout << msg << std::endl;
        return 1;
    }
    catch (...) {
        std::cout << "Unexpected exception" << std::endl;
        return 1;
    }
    
    return 0;
}