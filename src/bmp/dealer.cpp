#include "dealer.h"
#include <iostream>



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

        // log
        std::cout << "valid bitmap detected" << std::endl;

        // detect format type
        int formatType = detectFormatType(binary, begin);

        // OS / 2 bitmap
        if (formatType == OS_2_BITMAP) {
            // log
            std::cout << "OS / 2 bitmap format detected" << std::endl;
            loadCoreHeader(binary);

        // Windows bitmap
        } else if (formatType == WINDOWS_BITMAP) {
            // log
            std::cout << "Windows bitmap format detected" << std::endl;
            loadInfoHeader(binary);

        // unknown format
        } else {
            return;
        }

        BmpFileHeader fheader(binary);

        //log
        std::cout << fheader.toString() << std::endl;

        // load image
        loadImage(binary, fheader.offset);
    }


    // get image matrix
    void BmpDealer::getImage (Canvas *container) const {
        container->copy(image);
    };


    // get palette
    void BmpDealer::getPalette (std::vector<SimpleColor> *container) const {

        // prepare memory
        container->resize(palette.size());

        // copy data
        copy(palette.begin(), palette.end(), container->begin());
    };


    // get binary
    void BmpDealer::getBinary (
            std::vector<unsigned char> *container, int type) const {

        // delete container content
        container->clear();

        // get image binary
        std::vector<unsigned char> imgBinary;
        dumpImage(&imgBinary);

        // get infomation header
        std::vector<unsigned char> info;

        // encode OS / 2 bitmap
        if (type == OS_2_BITMAP) {
            BmpCoreHeader cheader(image.cols(), image.rows(), bitCount);
            cheader.toBinary(&info);

            // container
            SimpleColor color;
            const int paletteLength = 1 << bitCount;

            // dump pelette
            for ( int i = 0; i < paletteLength; i++ ) {
                if (i < palette.size()) {
                    color = palette[i];

                // case of index is not found in palette
                } else {
                    color = SimpleColor();
                }

                info.push_back(color.blue);
                info.push_back(color.green);
                info.push_back(color.red);
            }

        // encode windows bitmap
        } else {
            BmpInfoHeader iheader(image.cols(), image.rows(), bitCount);
            iheader.clrUsed = palette.size();
            iheader.cirImportant = palette.size();
            iheader.sizeImage = imgBinary.size();

            iheader.toBinary(&info);

            // container
            SimpleColor color;
            const int paletteLength = 1 << bitCount;

            // dump pelette
            for ( int i = 0; i < paletteLength; i++ ) {
                if (i < palette.size()) {
                    color = palette[i];

                // case of index is not found in palette
                } else {
                    color = SimpleColor();
                }

                info.push_back(color.blue);
                info.push_back(color.green);
                info.push_back(color.red);
                info.push_back(static_cast<unsigned char>(0));
            }
            std::cout << iheader.toString() << std::endl;
        }

        // set file header
        BmpFileHeader fheader(
                BmpFileHeader::length + info.size() + imgBinary.size(),
                BmpFileHeader::length + info.size());
        fheader.toBinary(container);
        std::cout << fheader.toString() << std::endl;

        // concat infomation header
        container->insert(container->end(), info.begin(), info.end());

        // concat image data
        container->insert(
                container->end(), imgBinary.begin(), imgBinary.end());

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

        // log
        std::cout << cheader.toString() << std::endl;
    };


    // load Windows header
    void BmpDealer::loadInfoHeader (
            const std::vector<unsigned char> &binary, int begin) {

        // get infomation header
        BmpInfoHeader iheader(binary, begin);

        // log
        std::cout << iheader.toString() << std::endl;

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

        // 1 bit bitmap
        if (bitCount == 1) {
            loadImage1bit(binary, begin);

        // 4 bit bitmap
        } else if (bitCount == 4) {
            loadImage4bit(binary, begin);

        // 8 bit bitmap
        } else if (bitCount == 8) {
            loadImage8bit(binary, begin);

        // 24, 32 bit bitmap
        } else if (bitCount == 24 || bitCount == 32) {
            loadImageFullColor(binary, begin);
        }

    };


    // 1 bit image loader
    void BmpDealer::loadImage1bit (
            const std::vector<unsigned char> &binary, int begin) {

        // get palette size
        const int color_size = palette.size();

        // palette index container
        int pidx;
        int idx = begin;

        // container
        std::vector<unsigned char> buffer;
        buffer.reserve(8);

        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j * 8 < image.cols(); j++ ) {

                // get palette index list
                convertColor1bit(binary[idx], &buffer);

                for ( int k = 0; k < 8; k++ ) {
                    // get palette index
                    pidx = static_cast<int>(buffer[k]);

                    // case of invalid index detected
                    if (pidx >= color_size) {
                        pidx = 0;
                    }

                    image.set(i, j * 8 + k, palette[pidx]);
                }

                // update index
                idx++;
            }
        }

    };


    // 4 bit image loader
    void BmpDealer::loadImage4bit (
            const std::vector<unsigned char> &binary, int begin) {

        // get palette size
        const int color_size = palette.size();

        // palette index container
        int pidx;
        int idx = begin;

        // container
        std::vector<unsigned char> buffer;
        buffer.reserve(2);

        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j * 2 < image.cols(); j++ ) {

                // get palette index list
                convertColor1bit(binary[idx], &buffer);

                for ( int k = 0; k < 2; k++ ) {
                    // get palette index
                    pidx = static_cast<int>(buffer[k]);

                    // case of invalid index detected
                    if (pidx >= color_size) {
                        pidx = 0;
                    }

                    image.set(i, j * 2 + k, palette[pidx]);
                }

                // update index
                idx++;
            }
        }

    };


    // 8 bit image loader
    void BmpDealer::loadImage8bit (
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


    // translate image
    void BmpDealer::dumpImage (
            std::vector<unsigned char> *container) const {

        // 1 bit bitmap
        if (bitCount == 1) {
            dumpImage1bit(container);

        // 4 bit bitmap
        } else if (bitCount == 4) {
            dumpImage4bit(container);

        // 8 bit bitmap
        } else if (bitCount == 8) {
            dumpImage8bit(container);

        // 24 bit bitmap
        } else if (bitCount == 24) {
            dumpImage24bit(container);

        // 32 bit bitmap
        } else if (bitCount == 32) {
            dumpImage32bit(container);
        }

    };


    // 1 bit image translator
    void BmpDealer::dumpImage1bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        // how many bytes 1 column has
        int width_bytes = image.cols() / 8 + 1;
        std::vector<unsigned char> buffer;
        buffer.reserve(8);

        // set binary
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < width_bytes; j++ ) {

                // initialize buffer
                buffer.clear();

                for ( int k = 0; k < 8; k++ ) {

                    // exist pixel
                    if (j * 8 + k < image.cols()) {
                        buffer.push_back(
                                allotPalette(image.get(i, j * 8 + k),
                                    palette));

                    // padding
                    } else {
                        buffer.push_back(static_cast<unsigned char>(0));

                    }

                }

                // collect data and store
                container->push_back(collectColor1bit(buffer));
            }
        }

    };


    // 4 bit image translator
    void BmpDealer::dumpImage4bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        // how many bytes 1 column has
        int width_bytes = image.cols() / 2 + 1;
        std::vector<unsigned char> buffer;
        buffer.reserve(2);

        // set binary
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < width_bytes; j++ ) {

                // initialize buffer
                buffer.clear();

                for ( int k = 0; k < 2; k++ ) {

                    // exist pixel
                    if (j * 2 + k < image.cols()) {
                        buffer.push_back(
                                allotPalette(image.get(i, j * 2 + k),
                                    palette));

                    // padding
                    } else {
                        buffer.push_back(static_cast<unsigned char>(0));

                    }

                }

                // collect data and store
                container->push_back(collectColor1bit(buffer));
            }
        }

    };


    // 8 bit image translator
    void BmpDealer::dumpImage8bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        // set binary
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < image.cols(); j++ ) {
                container->push_back(
                        allotPalette(image.get(i, j), palette));
            }
        }

    };


    // 24 bit image translator
    void BmpDealer::dumpImage24bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        SimpleColor color;

        // dump image
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < image.cols(); j++ ) {
                color = image.get(i, j);

                container->push_back(color.blue);
                container->push_back(color.green);
                container->push_back(color.red);
            }
        }

    };


    // 32 bit image translator
    void BmpDealer::dumpImage32bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        SimpleColor color;

        // reserved value
        unsigned char zero = static_cast<unsigned char>(0);

        // dump image
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < image.cols(); j++ ) {
                color = image.get(i, j);

                container->push_back(color.blue);
                container->push_back(color.green);
                container->push_back(color.red);
                container->push_back(zero);
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
