/*
 * File:   PrepareFaces.cpp
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#define DEFAULT_CASCADE_PATH "cascades/haarcascade_frontalface_default.xml"
#define ORIGINALS_LIST "obama_raw/list"
#define OUTPUT_DIR "obama_faces"
#define OUTPUT_LIST "list"
#define FACE_SIZE Size(150,150)

#include <cstdlib>
#include <fstream>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "FaceDetector.h"

using namespace std;
using namespace cv;

void read_input_list(const string &list_path, vector<Mat> &images) {
    ifstream file(list_path.c_str());
    string path;
    while (getline(file, path)) {
        images.push_back(imread(path));
    }
}

