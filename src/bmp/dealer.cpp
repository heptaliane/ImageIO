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


    // initialized by Canvas image matrix
    BmpDealer::BmpDealer (
            const Canvas &imgCanvas, unsigned short bitCount) {

        setBitCount(bitCount);
        setImage(imgCanvas);
    };



    // set image matrix
    void BmpDealer::setImage (const Canvas &newImage) {

        image.copy(newImage);
    };


    // set palette
    void BmpDealer::setPalette (const std::vector<SimpleColor> &newPalette) {

        // delete old palette
        palette.clear();

        // push new palette data
        for ( int i = 0; i < (1 << bitCount); i++ ) {
            palette.push_back(newPalette[i]);
        }
    };


    // set bit count
    void BmpDealer::setBitCount (unsigned short newBitCount) {

        bitCount = newBitCount;

        // change palette size
        if (bitCount < 24) {
            palette.resize(1 << bitCount);
        }
    };


    // set compression type
    void BmpDealer::setCompression (unsigned int newCompression) {
        compression = newCompression;
    };

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

        BmpFileHeader fheader(binary);

        // load image
        loadImage(binary, fheader.offset);
    };


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

            // set palette
            std::vector<unsigned char> pal;
            dumpPalette(&pal, OS_2_BITMAP);
            info.insert(info.end(), pal.begin(), pal.end());

        // encode windows bitmap
        } else {
            BmpInfoHeader iheader(image.cols(), image.rows(), bitCount);
            iheader.clrUsed = palette.size();
            iheader.cirImportant = palette.size();
            iheader.sizeImage = imgBinary.size();
            iheader.toBinary(&info);

            // set palette
            std::vector<unsigned char> pal;
            dumpPalette(&pal, WINDOWS_BITMAP);
            info.insert(info.end(), pal.begin(), pal.end());
        }

        // set file header
        BmpFileHeader fheader(
                BmpFileHeader::length + info.size() + imgBinary.size(),
                BmpFileHeader::length + info.size());
        fheader.toBinary(container);

        // concat infomation header
        container->insert(container->end(), info.begin(), info.end());

        // concat image data
        container->insert(
                container->end(), imgBinary.begin(), imgBinary.end());
    };


    //
    // binary loaders
    //


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
        loadPalette(binary, 1 << bitCount,
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

        // padding
        int padding = 4 - ((image.cols() + 7) / 8) % 4;
        if (padding == 4) {
            padding = 0;
        }

        for ( int i = static_cast<int>(image.rows()) - 1; i >= 0; i-- ) {
            for ( int j = 0; j * 8 < static_cast<int>(image.cols()); j++ ) {

                // get palette index list
                convertColor1bit(binary[idx], &buffer);

                for ( int k = 0; k < 8; k++ ) {
                    // get palette index
                    pidx = static_cast<int>(buffer[k]);

                    // case of invalid index detected
                    if (pidx >= color_size) {
                        pidx = 0;
                    }

                    // save image value
                    image.set(i, j * 8 + k, palette[pidx]);
                }

                // update index
                idx++;
            }
            // apply padding
            idx += padding;
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

        // padding
        int padding = 4 - ((image.cols() + 1) / 2) % 4;
        if (padding == 4) {
            padding = 0;
        }

        for ( int i = static_cast<int>(image.rows()) - 1; i >= 0; i-- ) {
            for ( int j = 0; j * 2 < static_cast<int>(image.cols()); j++ ) {

                // get palette index list
                convertColor4bit(binary[idx], &buffer);

                for ( int k = 0; k < 2; k++ ) {
                    // get palette index
                    pidx = static_cast<int>(buffer[k]);

                    // case of invalid index detected
                    if (pidx >= color_size) {
                        pidx = 0;
                    }

                    // save image value
                    image.set(i, j * 2 + k, palette[pidx]);
                }

                // update index
                idx++;
            }

            // apply padding
            idx += padding;
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

        // put padding so that it is an integral multiple of 4 bytes
        int padding = 4 - image.cols() % 4;
        if (padding == 4) {
            padding = 0;
        }

        for ( int i = static_cast<int>(image.rows() - 1); i >= 0; i-- ) {
            for ( int j = 0; j < static_cast<int>(image.cols()); j++ ) {
                // get palette index
                pidx = static_cast<int>(binary[idx]);

                // case of invalid index detected
                if (pidx >= color_size) {
                    pidx = 0;
                }

                // save value
                image.set(i, j, palette[pidx]);

                // update index
                idx++;
            }
            // apply padding
            idx += padding;
        }
    };


    // 24, 32 bit image loader
    void BmpDealer::loadImageFullColor (
            const std::vector<unsigned char> &binary, int begin) {

        SimpleColor color;
        int idx = begin;

        // 24 bitmap color is 3 byte, 32 is 4 byte
        const int color_size = (bitCount == 24) ? 3 : 4;

        // put padding so that it is an integral multiple of 4 bytes
        int padding = 4 - (image.cols() * 3) % 4;
        if (padding == 4 || bitCount == 32) {
            padding = 0;
        }

        // set image
        for ( int i = static_cast<int>(image.rows() - 1); i >= 0; i-- ) {
            for ( int j = 0; j < static_cast<int>(image.cols()); j++ ) {
                color.blue = binary[idx];
                color.green = binary[idx + 1];
                color.red = binary[idx + 2];

                // save image
                image.set(i, j, color);

                // update index
                idx += color_size;
            }
            // apply padding
            idx += padding;
        }
    };


    //
    // binary dumpers
    //


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
        int width_bytes = (image.cols() + 7) / 8;
        std::vector<unsigned char> buffer;
        buffer.reserve(8);

        // padding
        int padding = 4 - ((image.cols() + 7) / 8) % 4;
        if (padding == 4) {
            padding = 0;
        }

        // set binary
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < width_bytes; j++ ) {
                // initialize buffer
                buffer.clear();

                for ( int k = 0; k < 8; k++ ) {
                    // exist pixel
                    if (j * 8 + k < static_cast<int>(image.cols())) {
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

            // padding
            for ( int l = 0; l < padding; l++ ) {
                container->push_back(static_cast<unsigned char>(0));
            }
        }

    };


    // 4 bit image translator
    void BmpDealer::dumpImage4bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        // how many bytes 1 column has
        int width_bytes = (image.cols() + 1) / 2;
        std::vector<unsigned char> buffer;
        buffer.reserve(2);

        // padding
        int padding = 4 - ((image.cols() + 1) / 2) % 4;
        if (padding == 4) {
            padding = 0;
        }

        // set binary
        for ( int i = image.rows() - 1; i >= 0; i-- ) {
            for ( int j = 0; j < width_bytes; j++ ) {
                // initialize buffer
                buffer.clear();

                for ( int k = 0; k < 2; k++ ) {
                    // exist pixel
                    if (j * 2 + k < static_cast<int>(image.cols())) {
                        buffer.push_back(
                                allotPalette(image.get(i, j * 2 + k),
                                    palette));

                    // padding
                    } else {
                        buffer.push_back(static_cast<unsigned char>(0));
                        continue;
                    }
                }

                // collect data and store
                container->push_back(collectColor4bit(buffer));
            }

            // padding
            for ( int l = 0; l < padding; l++ ) {
                container->push_back(static_cast<unsigned char>(0));
            }
        }
    };


    // 8 bit image translator
    void BmpDealer::dumpImage8bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        // put padding so that it is an integral multiple of 4 bytes
        int padding = 4 - image.cols() % 4;
        if (padding == 4) {
            padding = 0;
        }

        // set binary
        for ( int i = static_cast<int>(image.rows()) - 1; i >= 0; i-- ) {
            for ( int j = 0; j < static_cast<int>(image.cols()); j++ ) {
                container->push_back(
                        allotPalette(image.get(i, j), palette));
            }

            // padding
            for ( int k = 0; k < padding; k++ ) {
                container->push_back(static_cast<unsigned char>(0));
            }
        }
    };


    // 24 bit image translator
    void BmpDealer::dumpImage24bit (
            std::vector<unsigned char> *container) const {

        // delete container content
        container->clear();

        // put padding so that it is an integral multiple of 4 bytes
        int padding = 4 - (image.cols() * 3) % 4;
        if (padding == 4) {
            padding = 0;
        }

        SimpleColor color;

        // dump image
        for ( int i = static_cast<int>(image.rows()) - 1; i >= 0; i-- ) {
            for ( int j = 0; j < static_cast<int>(image.cols()); j++ ) {
                color = image.get(i, j);

                container->push_back(color.blue);
                container->push_back(color.green);
                container->push_back(color.red);
            }

            // padding
            for ( int k = 0; k < padding; k++ ) {
                container->push_back(static_cast<unsigned char>(0));
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
        for ( int i = static_cast<int>(image.rows()) - 1; i >= 0; i-- ) {
            for ( int j = 0; j < static_cast<int>(image.cols()); j++ ) {
                color = image.get(i, j);

                container->push_back(color.blue);
                container->push_back(color.green);
                container->push_back(color.red);
                container->push_back(zero);
            }
        }
    };


    // translate palette
    void BmpDealer::dumpPalette (
            std::vector<unsigned char> *container, int type) const {

        // delete container content
        container->clear();

        // no palette is required for full color bitmap
        if (bitCount >= 24) {
            return;
        }

        // container
        SimpleColor color;
        const int paletteLength = 1 << bitCount;

        // dump pelette
        for ( int i = 0; i < paletteLength; i++ ) {
            if (i < static_cast<int>(palette.size())) {
                color = palette[i];

            // case of index is not found in palette
            } else {
                color = SimpleColor();
            }

            container->push_back(color.blue);
            container->push_back(color.green);
            container->push_back(color.red);

            // windows bitmap requires reserved space
            if (type == WINDOWS_BITMAP) {
                container->push_back(static_cast<unsigned char>(0));
            }
        }
    };


    //
    // format detector
    //


    // check if file type is BMP
    bool BmpDealer::check (
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
