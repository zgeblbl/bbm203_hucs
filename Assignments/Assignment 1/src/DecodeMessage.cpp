// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <iostream>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}

std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {
    std::string binaryString;
    for (const std::pair<int, int> pixel : edgePixels) {
        int pixelValue = image.get_data(pixel.first, pixel.second);
        int lsb = pixelValue & 1;
        binaryString += std::to_string(lsb);
    }
    std::string padding;
    if (binaryString.length()%7 != 0){
        for (int i=0; i<7-(binaryString.length()%7); i++) {
            padding+="0";
        }
    }
    padding+=binaryString;
    std::string message;
    for (int i = 0; i < padding.length(); i += 7) {
        std::string byte = padding.substr(i, 7);
        int ascii = std::stoi(byte, nullptr, 2);
        if (ascii<=32) {
            ascii += 33;
        }else if (ascii>=127) {
            ascii = 126;
        }
        char ch = static_cast<char>(ascii);
        message += ch;
    }
    return message;
}
