#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {

}

ImageSharpening::~ImageSharpening(){

}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {
    double** kernel = new double*[3];
    for (int i = 0; i < 3; i++) {
        kernel[i] = new double[3];
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = 1.0;
        }
    }
    Convolution conv(kernel, 3, 3, 1, true);
    ImageMatrix convMatrix = conv.convolve(input_image);
    ImageMatrix blurredImage = convMatrix*(1/9.0);
    ImageMatrix SharpImg = input_image + ((input_image - blurredImage)*k);
    for (int i = 0; i < SharpImg.get_height(); ++i) {
        for (int j = 0; j < SharpImg.get_width(); ++j) {
            if (SharpImg.get_data(i,j)>255){
                SharpImg.get_data()[i][j] = 255;
            }if (SharpImg.get_data(i,j)<0){
                SharpImg.get_data()[i][j] = 0;
            }
        }
    }
    for (int i = 0; i < 3; i++) {
        delete[] kernel[i];
    }
    delete[] kernel;
    return SharpImg;
}
