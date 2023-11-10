#include "ImageProcessor.h"
#include "Convolution.h"
#include "ImageSharpening.h"
#include "EdgeDetector.h"
#include "DecodeMessage.h"
#include "EncodeMessage.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}
ImageSharpening imageSharpening;
EdgeDetector edgeDetector;
DecodeMessage decodeMessage;
EncodeMessage encodeMessage;

std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {
    ImageMatrix sharpImg = imageSharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edgePixels = edgeDetector.detectEdges(sharpImg);
    std::string message = decodeMessage.decodeFromImage(sharpImg, edgePixels);
    return message;
}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {
    ImageMatrix sharpImg = imageSharpening.sharpen(img, 2);
    std::vector<std::pair<int, int>> edgePixels = edgeDetector.detectEdges(sharpImg);
    ImageMatrix encodedImg = encodeMessage.encodeMessageToImage(img, message, edgePixels);
    return encodedImg;
}
