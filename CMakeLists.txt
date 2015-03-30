cmake_minimum_required(VERSION 2.8)
project( ImageRetrieval )
list( APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}" )
find_package( OpenCV REQUIRED )
find_package( Boost REQUIRED COMPONENTS filesystem system serialization )
set(Boost_USE_STATIC_LIBS        ON)

set(SOURCES SurfFeatureExtractor.cpp ImageDB.cpp IndexInfo.cpp MedianImagePreprocessor.cpp StripeRemovalPreprocessor.cpp CombinedImagePreprocessor.cpp DerotationImagePreprocessor.cpp main.cpp)

add_executable( ImageRetrieval ${SOURCES} )
target_link_libraries(ImageRetrieval)
target_link_libraries( ImageRetrieval ${OpenCV_LIBS} )
target_link_libraries( ImageRetrieval ${Boost_LIBRARIES} )
