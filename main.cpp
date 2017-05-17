#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>
#include <fstream>
#include <sstream>

#include <face/facerec.hpp>
#include <face/predict_collector.hpp>

#include "SerialCommunicator.hpp"

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

int main()
{
    Ptr<LBPHFaceRecognizer> model = createLBPHFaceRecognizer();
    vector<Mat> images;
    vector<int> labels;
    try {
        read_csv("imgloc.csv", images, labels);
    } catch (cv::Exception& e) {
        cerr << "Error opening file \"" << "imgloc.csv" << "\". Reason: " << e.msg << endl;
        // nothing more we can do
        exit(1);
    }
    if(images.size() <= 1) {
        string error_message = "This demo needs at least 2 images to work. Please add more images to your data set!";
        CV_Error(CV_StsError, error_message);
    }

    model->train(images, labels);

    SerialCommunicator *serialCommunicator = new SerialCommunicator();

    while(true)
    {
        while(!serialCommunicator->receive()){}

        VideoCapture picStream(0);
        picStream.open(0);
        if(!picStream.isOpened()) {
            cerr << "Error opening camera" << endl;
            exit(1);
        }
        Mat picFrame;
        picStream >> picFrame;
        imwrite("test.jpg", picFrame);
        picStream.release();

        Mat testSample = imread("test.jpg", 0);
        int predictedLabel = -1;
        double confidence = 0.0;
        model->setThreshold(100.0);
        model->predict(testSample, predictedLabel, confidence);

        cout << "Predicted Sample: " << predictedLabel << "\r\n";
        cout << "Confidence: " << confidence << "\r\n";

        char response = 'Y';

        if(confidence > 30)
            response = 'N';

        serialCommunicator->send(response);
    }
}
