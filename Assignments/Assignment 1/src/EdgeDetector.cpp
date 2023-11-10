#include "EdgeDetector.h"
#include <cmath>
#include <iostream>

// Default constructor
EdgeDetector::EdgeDetector() {

}

// Destructor
EdgeDetector::~EdgeDetector() {

}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {
    std::vector<std::pair<int, int>> edge_points;
    int height = input_image.get_height();
    int width = input_image.get_width();
    double **Gx;
    Gx = new double *[3];
    for (int i = 0; i < 3; i++) {
        Gx[i] = new double[3];
    }
    Gx[0][0] = -1.0;
    Gx[0][1] = 0.0;
    Gx[0][2] = 1.0;
    Gx[1][0] = -2.0;
    Gx[1][1] = 0.0;
    Gx[1][2] = 2.0;
    Gx[2][0] = -1.0;
    Gx[2][1] = 0.0;
    Gx[2][2] = 1.0;

    double **Gy;
    Gy = new double *[3];
    for (int i = 0; i < 3; i++) {
        Gy[i] = new double[3];
    }
    Gy[0][0] = -1.0;
    Gy[0][1] = -2.0;
    Gy[0][2] = -1.0;
    Gy[1][0] = 0.0;
    Gy[1][1] = 0.0;
    Gy[1][2] = 0.0;
    Gy[2][0] = 1.0;
    Gy[2][1] = 2.0;
    Gy[2][2] = 1.0;

    Convolution convX(Gx, 3, 3, 1, true);
    ImageMatrix Ix = convX.convolve(input_image);
    Convolution convY(Gy, 3, 3, 1, true);
    ImageMatrix Iy = convY.convolve(input_image);
    std::vector<std::pair<int, int>> edgePixels;
    double sum=0;
    int num = 0;
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            sum+=sqrt(pow((Ix.get_data()[i][j]), 2) + pow((Iy.get_data()[i][j]), 2));
            num++;
        }
    }
    double threshold = sum / (height*width);
    for (int i=0; i<height; i++){
        for (int j=0; j<width; j++){
            if (threshold < sqrt(pow((Ix.get_data()[i][j]), 2) + pow((Iy.get_data()[i][j]), 2))){
                edgePixels.push_back(std::make_pair(i, j));
            }
        }
    }
    for (int i = 0; i < 3; ++i) {
        delete[] Gx[i];
        delete[] Gy[i];
    }
    delete[] Gx;
    delete[] Gy;
    return edgePixels;
}

