#include "header.h"


namespace {


// calculate CRC
unsigned int calcCRC (const std::vector<unsigned char> &data) {

    unsigned int crc = 0xFFFFFFFF;
    const unsigned int magic = 0xEDB88320;
    unsigned int table[256];
    unsigned int i, j, b, value;

    // calculate CRC table
    for ( i = 0; i < 256; i++ ) {
        value = i;
        for ( j = 0; j < 8; j++ ) {
            b = (value & 1);
            value >>= 1;
            if (b) {
                value ^= magic;
            }
        }
        table[i] = value;
    }

    // genelate CRC
    for ( i = 0; i < data.size(); i++ ) {
        crc = table[(crc ^ data[i]) & 0xff] ^ (crc >> 8);
    }

    return crc;
}

}

namespace imgio {



//
// PNG Image Header
//
// common to all png format that locate just after signature
//

//
// constant
//

const unsigned int PngImageHeader::length = 13;

// chunk type is IHDR
const unsigned char PngImageHeader::headerType[4] = {
    0x49, 0x48, 0x44, 0x52
};

// at present compression type is only 0
const unsigned char PngImageHeader::compression = 0;

// at present filter type is only 0
const unsigned char PngImageHeader::filter = 0;


// used when invalid color type and bit depth pair detected
// common to all color type
const unsigned char PngImageHeader::defaultBitCount = 8;


// convert to binary
void PngImageHeader::toBinary (std::vector<unsigned char> *container) const {

    // prepare buffer
    std::vector<unsigned char> buffer(4);

    // delete container content
    container->clear();

    // store the header data
    conv::convert(length, &buffer);
    container->insert(container->end(), buffer.begin(), buffer.end());

    container->push_back(headerType[0]);
    container->push_back(headerType[1]);
    container->push_back(headerType[2]);
    container->push_back(headerType[3]);

    conv::convert(width, &buffer);
    container->insert(container->end(), buffer.begin(), buffer.end());

    conv::convert(height, &buffer);
    container->insert(container->end(), buffer.begin(), buffer.end());

    container->push_back(checkBitCount() ? bitCount : defaultBitCount);
    container->push_back(colorType);

    container->push_back(compression);
    container->push_back(filter);
    container->push_back(interlace);

    unsigned int crc = calcCRC(
        std::vector<unsigned char>(container->begin() + 4, container->end()));
    conv::convert(crc, &buffer);
    container->insert(container->end(), buffer.begin(), buffer.end());
}


// get log string
std::string PngImageHeader::toString () const {

    std::ostringstream oss;

    oss << "Image size: " << width << 'x' << height << '\n';
    oss << "bit depth: " << bitCount << '\n';
    oss << "color type: " << colorType << '\n';
    oss << "interlace: " << (interlace == 1 ? "true" : "false") << '\n';

    return oss.str();
}



//
// PNG Palette Header
//
// this is required when color type is 3
//

//
// constant
//

const unsigned char PngPaletteHeader::headerType[4] = {
    0x50, 0x4C, 0x54, 0x45
};


// initialized by palette data
PngPaletteHeader::PngPaletteHeader (
        const std::vector<SimpleColor> &imgpalette) {

    for ( int i = 0; i < imgpalette.size(); i++ ) {
        palette.push_back(imgpalette[i]);
    }
}


// initialized by raw binary
PngPaletteHeader::PngPaletteHeader (
        const std::vector<unsigned char> &binary, int begin) {

    unsigned int length = conv::dumpUInt(binary, begin + 4);

    SimpleColor color;
    int idx = begin + 8;

    // import palette data
    for (int i = 0; i * 3 < length; i++) {
        color.red = binary[idx];
        color.green = binary[idx + 1];
        color.blue = binary[idx + 2];

        palette.push_back(color);
        idx += 3;
    }
}


// copy constructor
PngPaletteHeader::PngPaletteHeader (const PngPaletteHeader &obj) {

    // prepare memory
    palette.resize(obj.palette.size());

    // copy data
    copy(obj.palette.begin(), obj.palette.end(), palette.begin());
}


// convert to binary
void PngPaletteHeader::toBinary (
        std::vector<unsigned char> *container) const {

    // prepare buffer
    std::vector<unsigned char> buffer(4);

    // delete container content
    container->clear();

    // store the header data
    container->push_back(headerType[0]);

    unsigned int length = palette.size() * 3;
    conv::convert(length, &buffer);
    container->insert(container->end(), buffer.begin(), buffer.end());

    container->push_back(headerType[0]);
    container->push_back(headerType[1]);
    container->push_back(headerType[2]);
    container->push_back(headerType[3]);

    for ( int i = 0; i < palette.size(); i++ ) {
        container->push_back(palette[i].red);
        container->push_back(palette[i].green);
        container->push_back(palette[i].blue);
    }

    unsigned int crc = calcCRC(
        std::vector<unsigned char>(container->begin() + 4, container->end()));
    container->push_back(crc);
}


// convert to string
std::string PngPaletteHeader::toString () const {

    std::ostringstream oss;

    oss << "Palette length: " << palette.size() << '\n';
    for ( int i = 0; i < palette.size(); i++ ) {
        oss << "palette " << i << ": " << palette[i].red << ", "
            << palette[i].green << ", " << palette[i].blue << '\n';
    }

    return oss.str();
}

}
