#include "dealer.h"



namespace imgio {



//
// BmpDealer
//
// analyze binary data of bitmap and acquire infomation
// including matrix of images.
// or generate binary data in bitmap format from a matrix of images.
//



    // initialized by raw binary
    BmpDealer::BmpDealer (
            const std::vector<unsigned char> &binary, int begin) {
        loadBinary(binary, begin);
    };



    // set image matrix
    void BmpDealer::setImage (const Canvas &newImage) {
        image.copy(newImage);
    }


    // set palette
    void BmpDealer::setPalette (const std::vector<SimpleColor> &newPalette) {

        // prepare memory
        palette.resize(newPalette.size());

        // copy data
        copy(newPalette.begin(), newPalette.end(), palette.begin());

    }


    // set bit count
    void BmpDealer::setBitCount (unsigned short newBitCount) {
        bitCount = newBitCount;
    }


    // set compression type
    void BmpDealer::setCompression (unsigned int newCompression) {
        compression = newCompression;
    }

    // load raw binary
    void BmpDealer::loadBinary (
            const std::vector<unsigned char> &binary, int begin) {

        // file type of the binary is not bitmap
        if (!check(binary)) {
            return;
        }

        // detect format type
        int formatType = detectFormatType(binary, begin);

        // OS / 2 bitmap
        if (formatType == OS_2_BITMAP) {
            loadCoreHeader(binary);

        // Windows bitmap
        } else if (formatType == WINDOWS_BITMAP) {
            loadInfoHeader(binary);

        // unknown format
        } else {
            return;
        }

        BmpFileHeader fHeader(binary);

        // load image
        loadImage(binary, fHeader.offset);
    }


    // get image matrix
    void BmpDealer::getImage (Canvas *container) const {
        container->copy(image);
    };


    // TODO
    // get binary
    void BmpDealer::getBinary (
            std::vector<unsigned char> *container, int type) const {

    };



    // load OS / 2 header
    void BmpDealer::loadCoreHeader (
            const std::vector<unsigned char> &binary, int begin) {

        // get core header
        BmpCoreHeader cheader(binary, begin);

        // set bit count
        bitCount = cheader.bitCount;

        // set compression
        compression = 0;

        // set image size
        image.resize(cheader.width, cheader.height);

        // load palette
        loadPalette(binary, 1 << bitCount,
                begin + BmpCoreHeader::length, OS_2_BITMAP);

    };


    // load Windows header
    void BmpDealer::loadInfoHeader (
            const std::vector<unsigned char> &binary, int begin) {

        // get infomation header
        BmpInfoHeader iheader(binary, begin);

        // set bit count
        bitCount = iheader.bitCount;

        // setCompression
        compression = iheader.compression;

        // set image size
        image.resize(iheader.width, iheader.height);

        // load palette
        loadPalette(binary, iheader.clrUsed,
                begin + BmpInfoHeader::length, WINDOWS_BITMAP);
    };


    // load palette
    void BmpDealer::loadPalette (
            const std::vector<unsigned char> &binary,
            int length, int begin, int type) {

        // 24 or 32 bit bitmap has no palette
        if (bitCount == 24 || bitCount == 32) {
            return;
        }

        // default palette byte count
        int color_size = 3;

        // Windows bitmap has reserved section in color
        if (type == WINDOWS_BITMAP) {
            color_size = 4;
        }

        // initialize palette
        palette.clear();

        SimpleColor color;
        int idx = begin;

        // set palette
        for ( int i = 0; i < length; i++ ) {
            color.blue = binary[idx];
            color.green = binary[idx + 1];
            color.red = binary[idx + 2];
            palette.push_back(color);

            // update index
            idx += color_size;
        }

    };


    // load image
    void BmpDealer::loadImage (
            const std::vector<unsigned char> &binary, int begin) {

        // full color bitmap
        if (bitCount == 24 || bitCount == 32) {
            loadImageFullColor(binary, begin);

        // bitmap with palette
        } else {
            loadImageWithPalette(binary, begin);
        }

    };


    // 24, 32 bit image loader
    void BmpDealer::loadImageFullColor (
            const std::vector<unsigned char> &binary, int begin) {

        SimpleColor color;
        int idx = 0;

        // 24 bitmap color is 3 byte, 32 is 4 byte
        const int color_size = (bitCount == 24) ? 3 : 4;

        // set image
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < image.cols(); j++ ) {
                color.blue = binary[idx];
                color.green = binary[idx + 1];
                color.red = binary[idx + 2];
                image.set(i, j, color);

                // update index
                idx += color_size;
            }
        }

    };


    // 1, 4, 8 bit image loader
    void BmpDealer::loadImageWithPalette(
            const std::vector<unsigned char> &binary, int begin) {

        // get palette size
        const int color_size = palette.size();

        // palette index container
        int pidx;
        int idx = begin;

        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < image.cols(); j++ ) {
                // get palette index
                pidx = static_cast<int>(binary[idx]);

                // case of invalid index detected
                if (pidx >= color_size) {
                    pidx = 0;
                }

                image.set(i, j, palette[pidx]);

                // update index
                idx++;
            }
        }

    };



    // check if file type is BMP
    inline bool BmpDealer::check (
            const std::vector<unsigned char> &binary, int begin) {

        return binary[begin] == BmpFileHeader::type[0]
            && binary[begin + 1] == BmpFileHeader::type[1];

    };


    // detect bitmap format type
    int BmpDealer::detectFormatType (
            const std::vector<unsigned char> &binary, int begin) {

        // get infomation header length
        unsigned int infoLength = conv::dumpUInt(
                binary, begin + BmpFileHeader::length);


        // same length as bitmap core header
        if (infoLength == BmpCoreHeader::length) {
            return OS_2_BITMAP;

        // same length as bitmap infomation header
        } else if (infoLength == BmpInfoHeader::length) {
            return WINDOWS_BITMAP;

        // no known bitmap format type found
        } else {
            return UNKNOWN_BITMAP;
        }

    };
}
