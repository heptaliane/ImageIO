#ifndef PNG_HEADER_H_
#define PNG_HEADER_H_

#include <vector>
#include <string>
#include <sstream>

#include "../converter.h"

namespace imgio {

static const unsigned char pngSignature[8] = {
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a
};


//
// PNG Image Header
//
// common to all png format that locate just after signature
//


class PngImageHeader {
public:


    // constant
    static const unsigned int length;
    static const unsigned char headerType[4];
    static const unsigned char compression;
    static const unsigned char filter;



    // default constructor
    PngImageHeader ():
        width(0), height(0), bitCount(8),
        colorType(2), interlace(1) {};


    // initialized by image size (width and height)
    PngImageHeader (unsigned int imgWidth, unsigned int imgHeight,
            unsigned char imgBitCount = 8, unsigned char imgColorType = 2) :
        width(imgWidth), height(imgHeight), bitCount(imgBitCount),
        colorType(imgColorType), interlace(1) {};


    // initialized by raw binary
    PngImageHeader (const std::vector<unsigned char> &binary, int begin = 0) :
        width(conv::dumpUInt(binary, begin + 8)),
        height(conv::dumpUInt(binary, begin + 12)),
        bitCount(conv::dumpUChar(binary, begin + 16)),
        colorType(conv::dumpUChar(binary, begin + 17)),
        interlace(conv::dumpUChar(binary, begin + 20)) {};


    // copy constructor
    PngImageHeader (const PngImageHeader &obj) :
        width(obj.width), height(obj.height), bitCount(obj.bitCount),
        colorType(obj.colorType), interlace(obj.interlace) {};



    // convert to binary
    void toBinary (std::vector<unsigned char> *container) const;


    // get log string
    std::string toString () const;


    // entity
    unsigned int width;
    unsigned int height;
    unsigned char bitCount;
    unsigned char colorType;
    unsigned char interlace;


private:


    // constant
    static const unsigned char defaultBitCount;


    // check if bit depth is valid with color type
    bool checkBitCount () const {
        return (
            (colorType == 0 && (
                bitCount == 1 || bitCount == 2 || bitCount == 4 ||
                bitCount == 8 || bitCount == 16)) ||
            (colorType == 2 && (bitCount == 8 || bitCount == 16) ||
            (colorType == 3 && (
                bitCount == 1 || bitCount == 2 ||
                bitCount == 4 || bitCount == 8)) ||
            (colorType == 4 && (bitCount == 8 || bitCount == 16) ||
            (colorType == 6 && (bitCount == 8 || bitCount == 16)
        );
    }

};

}

#endif /* PNG_HEADER_H_ */
