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



}
