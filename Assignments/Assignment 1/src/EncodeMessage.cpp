#include "EncodeMessage.h"
#include <cmath>
#include <iostream>
#include <bitset>



// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {

}
int getFibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    int result=0;
    for (int i=2; i <= n; i++) {
        result=a+b;
        a=b;
        b = result;
    }
    return result;
}
bool isPrime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}
// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {
    ImageMatrix encodedImage = img;
    std::string fibMessage;
    for (int i = 0; i<message.length(); i++) {
        if (isPrime(i)) {
            int newCharVal = static_cast<int>(static_cast<unsigned char>(message[i])) + getFibonacci(static_cast<int>(message[i]));
            if (newCharVal >= 127) {
                newCharVal = 126;
            }if (newCharVal<=32){
                newCharVal+=33;
            }
            fibMessage+=(static_cast<char>(newCharVal));
        }else{
            fibMessage+=message[i];
        }
    }
    std::string shiftedMessage = fibMessage.substr(fibMessage.length() - (fibMessage.length()/2)) + fibMessage.substr(0, (fibMessage.length() - (fibMessage.length()/2)));
    std::string binaryStr;
    for (char c : shiftedMessage) {
        std::bitset<7> binaryChar(c);
        binaryStr += binaryChar.to_string();
    }
    int binaryIndex=0;
    for (const std::pair<int, int> pair : positions){
        if (binaryIndex >= binaryStr.length()) {
            break;
        }
        int rowIndex = pair.first;
        int colIndex = pair.second;
        encodedImage.get_data()[rowIndex][colIndex] = (static_cast<int>(encodedImage.get_data()[rowIndex][colIndex]) & 0xFFFFFFFE) | (binaryStr[binaryIndex] - '0');
        binaryIndex++;
    }
    return encodedImage;
}
