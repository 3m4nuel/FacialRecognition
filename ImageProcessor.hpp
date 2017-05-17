#ifndef IMAGEPROCESSOR_HPP
#define IMAGEPROCESSOR_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <face/facerec.hpp>
#include <face/predict_collector.hpp>


const char *TRAINING_DATA_CONFIG = "imgloc.csv";
const double VALID_CONF_LIMIT = 50.0;

using namespace cv;
using namespace std;
using namespace face;

class ImageProcessor
{
    public:
        ImageProcessor();
        void takePicture();
        bool isValidUser();
    private:
        Ptr<LBPHFaceRecognizer> model;
};

#endif // IMAGEPROCESSOR_HPP
