#Image Retrieval task for WebKontrol
## Install
Use cmake to install.

OpenCV and Boost are needed.
## Usage
type in command line: ./ImageRetrieval  image_base query_image force_build
* **image_base** - folder contains index images
* **query_image** - image to search for similar in image database
* **force_build** - optional, use 1 for build index event if there is an existing saved index. 

## Algorithm description
0. Look for an existing builded index in **image_base** folder (if there is no parameter **force_build**). If index is found then read it and go to step 3.
1. Read images in **image_base** folder, find and extract feature descriptors using SURF.
2. Build an KD-tree index using OpenCV::flann and save it in **image_base** folder.
3. Read **query_image**, preprocess it (apply median filter, image derotation and stripes removal), find and extract feature descriptors using SURF.
4. Search in index for **k** (I used 3) similar descriptors and calculate similarity rank based on distance between similar descriptors.
