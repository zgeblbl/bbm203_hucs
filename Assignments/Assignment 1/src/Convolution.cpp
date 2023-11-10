#include <iostream>
#include "ImageMatrix.h"

#include "Convolution.h"

// Default constructor 
Convolution::Convolution() : customKernel(nullptr), kernelHeight(0), kernelWidth(0), stride(1), padding(false)  {
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) :
        customKernel(customKernel), kernelHeight(kh), kernelWidth(kw), stride(stride_val), padding(pad) {
}

// Destructor
Convolution::~Convolution() {
}
// Copy constructor
Convolution::Convolution(const Convolution &other) :
        customKernel(nullptr), kernelHeight(0), kernelWidth(0), stride(1), padding(false) {
    if (other.customKernel) {
        customKernel = new double*[other.kernelHeight];
        for (int i = 0; i < other.kernelHeight; ++i) {
            customKernel[i] = new double[other.kernelWidth];
            for (int j = 0; j < other.kernelWidth; ++j) {
                customKernel[i][j] = other.customKernel[i][j];
            }
        }
    }
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {
    if (this == &other) {
        return *this;
    }
    if (customKernel) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] customKernel[i];
        }
        delete[] customKernel;
    }
    if (other.customKernel) {
        customKernel = new double*[other.kernelHeight];
        for (int i = 0; i < other.kernelHeight; ++i) {
            customKernel[i] = new double[other.kernelWidth];
            for (int j = 0; j < other.kernelWidth; ++j) {
                customKernel[i][j] = other.customKernel[i][j];
            }
        }
    }
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
    return *this;
}
// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    ImageMatrix img = input_image;
    int myConstant = 0;
    int inputHeight = input_image.get_height();
    int inputWidth = input_image.get_width();
    int newHeight = ((inputHeight - kernelHeight + (2 * padding)) / stride) + 1;
    int newWidth = ((inputWidth - kernelWidth + (2 * padding)) / stride) + 1;
    if (padding){
        double** paddedMatrix = new double*[inputHeight+2];
        for (int i = 0; i < inputHeight+2; i++) {
            paddedMatrix[i] = new double[inputWidth+2];
        }
        for (int i = 0; i < inputHeight+2; i++) {
            for (int j = 0; j < inputWidth+2; j++) {
                if (i < padding || i >= padding + inputHeight || j < padding || j >= padding + inputWidth) {
                    paddedMatrix[i][j]=0;
                } else {
                    int inputRow = i - padding;
                    int inputCol = j - padding;
                    paddedMatrix[i][j]=input_image.get_data(inputRow, inputCol);
                }
            }
        }
        inputHeight+=2;
        inputWidth+=2;
        ImageMatrix newInputMatrix(const_cast<const double **>(paddedMatrix), inputHeight, inputWidth);
        img = newInputMatrix;
        myConstant = 1;
        for (int i = 0; i < (inputHeight); i++) {
            delete[] paddedMatrix[i];
        }
        delete[] paddedMatrix;
    }
    double** tempMatrix = new double*[newHeight];
    for (int i = 0; i < newHeight; i++) {
        tempMatrix[i] = new double[newWidth];
    }
    for (int i = 0; i < newHeight; i++) {
        for (int j = 0; j < newWidth; j++) {
            double sum = 0.0;
            for (int m = 0; m < kernelHeight; m++) {
                for (int n = 0; n < kernelWidth; n++) {
                    int inputX = i * stride + m - padding + myConstant;
                    int inputY = j * stride + n - padding + myConstant;
                    if ((inputX >= 0)&&(inputX < inputHeight)&&(inputY >= 0)&&(inputY < inputWidth)) {
                        sum += img.get_data(inputX, inputY) * this->customKernel[m][n];
                    }
                }
            }
            tempMatrix[i][j] =sum;
        }
    }
    const double** constTempMatrix = const_cast<const double **>(tempMatrix);
    ImageMatrix convolvedMatrix(constTempMatrix, newHeight, newWidth);
    for (int i = 0; i < newHeight; i++) {
        delete[] tempMatrix[i];
    }
    delete[] tempMatrix;
    return convolvedMatrix;
}
