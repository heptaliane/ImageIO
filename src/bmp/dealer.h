#ifndef BMP_DEALER_H_
#define BMP_DEALER_H_

#include <vector>
#include <algorithm>

#include "../canvas.h"
#include "../imgutils.h"
#include "../converter.h"
#include "header.h"


// copy and assign guard macro
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName (const TypeName&); \
    void operator=(const TypeName&)



namespace imgio {



// Bitmap format type indentifier
static const int UNKNOWN_BITMAP = 0;
static const int OS_2_BITMAP = 1;
static const int WINDOWS_BITMAP = 2;


//
// BmpDealer
//
// analyze binary data of bitmap and acquire infomation
// including matrix of images.
// or generate binary data in bitmap format from a matrix of images.
//



class BmpDealer {
public:


    // default constructor
    BmpDealer () :
        bitCount(24), compression(0) {};


    // initialized by raw binary
    BmpDealer (const std::vector<unsigned char> &binary, int begin = 0);


    // initialized by Canvas image matrix
    BmpDealer (const Canvas &imgCanvas, unsigned short bitCount = 24);



    // set image matrix
    void setImage (const Canvas &newImage);


    // set palette
    void setPalette (const std::vector<SimpleColor> &newPalette);


    // set bit count
    void setBitCount(unsigned short newBitCount);


    // set compression type
    void setCompression(unsigned int newCompression);


    // load raw binary
    void loadBinary (const std::vector<unsigned char> &binary, int begin = 0);


    // get image matrix
    void getImage (Canvas *container) const;


    // get palette
    void getPalette (std::vector<SimpleColor> *container) const;


    // get binary
    void getBinary (
            std::vector<unsigned char> *container,
            int type = WINDOWS_BITMAP) const;


    // check if file type is BMP
    static bool check (
            const std::vector<unsigned char> &binary, int begin = 0);


private:


    // detect bitmap format type
    int detectFormatType (
            const std::vector<unsigned char> &binary, int begin = 0);


    // load OS / 2 header
    void loadCoreHeader (
            const std::vector<unsigned char> &binary,
            int begin = BmpFileHeader::length);


    // load Windows header
    void loadInfoHeader (
            const std::vector<unsigned char> &binary,
            int begin = BmpFileHeader::length);


    // load palette
    void loadPalette (
            const std::vector<unsigned char> &binary,
            int length, int begin, int type);


    // load image
    void loadImage (
            const std::vector<unsigned char> &binary, int begin);


    // 24, 32 bit image loader
    void loadImageFullColor (
            const std::vector<unsigned char> &binary, int begin);


    // 1 bit image loader
    void loadImage1bit (
            const std::vector<unsigned char> &binary, int begin);


    // 4 bit image loader
    void loadImage4bit (
            const std::vector<unsigned char> &binary, int begin);


    // 8 bit image loader
    void loadImage8bit (
            const std::vector<unsigned char> &binary, int begin);


    // translate image
    void dumpImage (
            std::vector<unsigned char> *container) const;


    // 1 bit image translator
    void dumpImage1bit (std::vector<unsigned char> *container) const;

    // 4 bit image translator
    void dumpImage4bit (std::vector<unsigned char> *container) const;

    // 8 bit image translator
    void dumpImage8bit (std::vector<unsigned char> *container) const;

    // 24 bit image translator
    void dumpImage24bit (std::vector<unsigned char> *container) const;

    // 32 bit image translator
    void dumpImage32bit (std::vector<unsigned char> *container) const;


    // contained data
    Canvas image;
    std::vector<SimpleColor> palette;
    unsigned short bitCount;
    unsigned int compression;


    // do not allow copy and assign this object
    DISALLOW_COPY_AND_ASSIGN (BmpDealer);
};


}
#endif /* BMP_DEALER_H_ */
