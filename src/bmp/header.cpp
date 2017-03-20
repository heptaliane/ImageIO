#include "header.h"


namespace imgio {


//
// Bmp File Header
//
//
// common to all bmp types, and always locate at the beginning of the file
//


// constant
const unsigned int BmpFileHeader::length = 14;

const unsigned char BmpFileHeader::type[2] = {
    'B', 'M'
};

const unsigned char BmpFileHeader::reserved1[2] = {
    static_cast<unsigned char>(0), static_cast<unsigned char>(0)
};

const unsigned char BmpFileHeader::reserved2[2] = {
    static_cast<unsigned char>(0), static_cast<unsigned char>(0)
};



// convert data to binary bitmap file header
void BmpFileHeader::toBinary (std::vector<unsigned char> *bin) const {

    // prepare buffer
    std::vector<unsigned char> buffer(4);

    // delete container content
    bin->clear();

    // store the header data
    bin->push_back(type[0]);
    bin->push_back(type[1]);

    conv::convert(size, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    bin->push_back(reserved1[0]);
    bin->push_back(reserved1[1]);

    bin->push_back(reserved2[0]);
    bin->push_back(reserved2[1]);

    conv::convert(offset, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

};


// convert data to std::string
std::string BmpFileHeader::toString () const {

    std::ostringstream oss;

    oss << "File size: " << size << '\n';
    oss << "Offset size: " << offset << '\n';

    return oss.str();
};



// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====



//
// Bitmap Core Header
//
// used in OS / 2 bitmap
// it is located after file header
//



// constant
const unsigned int BmpCoreHeader::length = 12;
const unsigned char BmpCoreHeader::planes[2] = {
    static_cast<unsigned char>(1), static_cast<unsigned char>(0)
};



// convert data to binary bitmap file header
void BmpCoreHeader::toBinary (std::vector<unsigned char> *bin) const {

    // prepare buffer
    std::vector<unsigned char> buffer(4);

    // delete container content
    bin->clear();

    // store the header data
    conv::convert(length, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(width, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(height, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    bin->push_back(planes[0]);
    bin->push_back(planes[1]);

    conv::convert(bitCount, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

};



// convert data to std::string
std::string BmpCoreHeader::toString () const {

    std::ostringstream oss;

    oss << "Image size: " << width << 'x' << height << '\n';
    oss << "bit count: " << bitCount << '\n';

    return oss.str();
};



// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====



//
// Bitmap Infomation Header
//
// used in windows bitmap
// it is located after file header
//


// constant
const unsigned int BmpInfoHeader::length = 40;
const unsigned char BmpInfoHeader::planes[2] = {
    static_cast<unsigned char>(1), static_cast<unsigned char>(0)
};



// convert data to binary bitmap file header
void BmpInfoHeader::toBinary (std::vector<unsigned char> *bin) const {

    // prepare buffer
    std::vector<unsigned char> buffer(4);

    // delete container content
    bin->clear();

    // store the header data
    conv::convert(length, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(width, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(height, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    bin->push_back(planes[0]);
    bin->push_back(planes[1]);

    conv::convert(bitCount, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(compression, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(sizeImage, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(xPixPerMeter, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(yPixPerMeter, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(clrUsed, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

    conv::convert(cirImportant, &buffer);
    bin->insert(bin->end(), buffer.begin(), buffer.end());

};


// convert data to std::string
std::string BmpInfoHeader::toString () const {

    std::ostringstream oss;

    oss << "Image size: " << width << 'x' << height << '\n';
    oss << "bit count: " << bitCount << '\n';
    oss << "compression type: " << compression << '\n';
    oss << "image length: " << sizeImage << " bytes" << '\n';
    oss << "pixel per meter: (" << xPixPerMeter << ',' << yPixPerMeter
        << ")\n";
    oss << "used palette: " << clrUsed << '\n';
    oss << "important palette: " << cirImportant << '\n';

    return oss.str();
};


}
