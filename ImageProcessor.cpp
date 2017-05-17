#include "ImageProcessor.hpp"

using namespace cv;
using namespace std;
using namespace face;

inline static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels, char separator = ';') {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) {
        string error_message = "No valid input file was given, please check the given filename.";
        CV_Error(CV_StsBadArg, error_message);
    }
    string line, path, classlabel;
    while (getline(file, line)) {
        stringstream liness(line);
        getline(liness, path, separator);
        getline(liness, classlabel);
        if(!path.empty() && !classlabel.empty()) {
            images.push_back(imread(path, 0));
            labels.push_back(atoi(classlabel.c_str()));
        }
    }
}

ImageProcessor::ImageProcessor()
{
    ImageProcessor::model = createLBPHFaceRecognizer();
    vector<Mat> images;
    vector<int> labels;
    try {
        read_csv(TRAINING_DATA_CONFIG, images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << TRAINING_DATA_CONFIG << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    if(images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }
    ImageProcessor::model->train(images, labels);
}

void ImageProcessor::takePicture()
{
    VideoCapture picStream(0);
    picStream.open(0);
    if(!picStream.isOpened()) {
        cerr << "Error opening camera" << endl;
        // nothing more we can do
        exit(1);
    }
    Mat picFrame;
    picStream >> picFrame;
    imwrite("test.jpg", picFrame);
    picStream.release();
}

bool ImageProcessor::isValidUser()
{
    Mat testSample = imread("test.jpg", 0);
    int predictedLabel = -1;
    double confidence = 0.0;
    model->setThreshold(100.0);
    model->predict(testSample, predictedLabel, confidence);

    if(confidence > VALID_CONF_LIMIT)
        return false;

    return true;
}

