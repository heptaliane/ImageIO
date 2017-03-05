#ifndef BMP_HEADER_H_
#define BMP_HEADER_H_

#include <vector>
#include <string>
#include <sstream>

#include "../converter.h"

namespace imgio {


//
// Bitmap File Header
//
//
// common to all bmp types, and always locate at the begining of the file
//



class BmpFileHeader {
public:


    // constant
    static const unsigned int length;
    static const unsigned char type[2];
    static const unsigned char reserved1[2];
    static const unsigned char reserved2[2];


    // default constructor
    BmpFileHeader () :
        size(0), offset(0) {};


    // initialized by file size and header + palette size
    BmpFileHeader (unsigned int bmpSize, unsigned int bmpOffset) :
        size(bmpSize), offset(bmpOffset) {};


    // initialized by raw binary
    explicit BmpFileHeader (
            const std::vector<unsigned char> &header, int begin = 0) :
        size(conv::dumpUInt(header, begin + 2)),
        offset(conv::dumpUInt(header, begin + 10)) {};


    // copy constructor
    BmpFileHeader (const BmpFileHeader &obj) :
        size(obj.size), offset(obj.offset) {};



    // convert data to binary bitmap file header
    void toBinary (std::vector<unsigned char> *bin) const;


    // convert data to std::string
    std::string toString () const;


    // entity
    unsigned int size;
    unsigned int offset;

};


//  ===== ===== ===== ===== ===== ===== ===== ===== ===== =====


//
// Bitmap Core Header
//
// used in OS / 2 bitmap
// it is located after file header
//



class BmpCoreHeader {
public:


    // constant
    static const unsigned int length;
    static const unsigned char planes[2];


    // default constructor
    BmpCoreHeader () :
        width(0), height(0), bitCount(24) {};


    // initialized by image size (width and height)
    BmpCoreHeader (short imgWidth, short imgHeight,
            unsigned short imgBitCount = 24) :
        width(imgWidth), height(imgHeight), bitCount(imgBitCount) {};


    // initialized by raw binary
    explicit BmpCoreHeader (
            const std::vector<unsigned char> &header, int begin = 0) :
        width(conv::dumpShort(header, begin + 2)),
        height(conv::dumpShort(header, begin + 4)),
        bitCount(conv::dumpUShort(header, begin + 10)) {};


    // copy constructor
    BmpCoreHeader (const BmpCoreHeader &obj) :
        width(obj.width), height(obj.height), bitCount(obj.bitCount) {};



    // convert data to binary bitmap file header
    void toBinary (std::vector<unsigned char> *bin) const;


    // convert data to std::string
    std::string toString () const;


    // entity
    short width;
    short height;
    unsigned short bitCount;

};


//  ===== ===== ===== ===== ===== ===== ===== ===== ===== =====


//
// Bitmap Infomation Header
//
// used in windows bitmap
// it is located after file header
//



class BmpInfoHeader {
public:


    // constant
    static const unsigned int length;
    static const unsigned char planes[2];


    // default constructor
    BmpInfoHeader () :
        width(0), height(0), bitCount(0),
        compression(0), sizeImage(0), xPixPerMeter(0), yPixPerMeter(0),
        clrUsed(0), cirImportant(0) {};


    // initialized by image size (width and height)
    BmpInfoHeader (int imgWidth, int imgHeight,
            unsigned short imgBitCount = 24) :
        width(imgWidth), height(imgHeight), bitCount(imgBitCount),
        compression(0), sizeImage(0), xPixPerMeter(0), yPixPerMeter(0),
        clrUsed(0), cirImportant(0) {};


    // initialized by raw binary
    explicit BmpInfoHeader (
            const std::vector<unsigned char> &header, int begin = 0) :
        width(conv::dumpInt(header, begin + 4)),
        height(conv::dumpInt(header, begin + 8)),
        bitCount(conv::dumpUShort(header, begin + 14)),
        compression(conv::dumpUInt(header, begin + 16)),
        sizeImage(conv::dumpUInt(header, begin + 20)),
        xPixPerMeter(conv::dumpInt(header, begin + 24)),
        yPixPerMeter(conv::dumpInt(header, begin + 28)),
        clrUsed(conv::dumpUInt(header, begin + 32)),
        cirImportant(conv::dumpUInt(header, begin + 36)) {};



    // copy constructor
    BmpInfoHeader (const BmpInfoHeader &obj) :
        width(obj.width), height(obj.height), bitCount(obj.bitCount),
        compression(obj.compression), sizeImage(obj.sizeImage),
        xPixPerMeter(obj.xPixPerMeter), yPixPerMeter(obj.yPixPerMeter),
        clrUsed(obj.clrUsed), cirImportant(obj.cirImportant) {};



    // convert data to binary bitmap file header
    void toBinary (std::vector<unsigned char> *bin) const;


    // convert data to std::string
    std::string toString () const;


    // entity
    int width;
    int height;
    unsigned short bitCount;
    unsigned int compression;
    unsigned int sizeImage;
    int xPixPerMeter;
    int yPixPerMeter;
    unsigned int clrUsed;
    unsigned int cirImportant;

};

}

#endif /* BMP_HEADER_H_ */
