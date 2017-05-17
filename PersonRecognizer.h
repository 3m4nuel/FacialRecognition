/*
 * File:   PersonRecognizer.h
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#ifndef PERSONRECOGNIZER_H
#define	PERSONRECOGNIZER_H

#define PERSON_LABEL 10 //some arbitrary label

#include <string>
#include "opencv2/imgproc.hpp"

#include <face/facerec.hpp>
#include <face/predict_collector.hpp>

using namespace std;
using namespace cv;

class PersonRecognizer {
public:
    PersonRecognizer(const vector<Mat> &imgs, int radius, int neighbors,
            int grid_x, int grid_y, double threshold);
    virtual ~PersonRecognizer();
    bool recognize(const Mat &face, double &confidence) const;
private:
    Ptr<face::FaceRecognizer> _model;
    Size _faceSize;
};

#endif	/* PERSONRECOGNIZER_H */

