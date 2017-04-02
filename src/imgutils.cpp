#include "imgutils.h"


namespace {

// square the value
int square (int value) {
    return value * value;
}

}


// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====



namespace imgio {


// detect most proper color from palette
unsigned char allotPalette (
        const SimpleColor &color, const std::vector<SimpleColor> &palette) {

    // minimum variance palette index ( initialized with 0 )
    unsigned char pidx = 0;

    // minimum variance ( initialized with maximum value )
    int minvar = 1 << 18;
    int var;

    // check with all color in the palette
    for ( int i = 0; i < static_cast<int>(palette.size()); i++ ) {

        // calculate variance
        var = square(static_cast<int>(palette[i].red)
                - static_cast<int>(color.red))
            + square(static_cast<int>(palette[i].green)
                - static_cast<int>(color.green))
            + square(static_cast<int>(palette[i].blue)
                - static_cast<int>(color.blue));

        // update "minvar"
        if (var < minvar) {
            minvar = var;
            pidx = i;
        }

        // case of same strict
        if (var == 0) {
            break;
        }

    }

    return pidx;

}


// 1 bit color data
unsigned char collectColor1bit (const std::vector<unsigned char> &binary) {

    // store to the 1 byte variable
    unsigned char value = 0;
    for ( int i = 0; i < 8; i++ ) {
        value += binary[i] << (7 - i);
    }

    return value;

};


void convertColor1bit (
        unsigned char value, std::vector<unsigned char> *container) {

    // delete container content
    container->clear();

    for ( int i = 7; i >= 0; i-- ) {
        container->push_back((value >> i) % 2);
    }

};


// 4 bit color data
unsigned char collectColor4bit (const std::vector<unsigned char> &binary) {

    // store to the 1 byte variable
    unsigned char value = binary[1] + (binary[0] << 4);

    return value;

};


void convertColor4bit (
        unsigned char value, std::vector<unsigned char> *container) {

    // delete container content
    container->clear();

    container->push_back((value >> 4) % (1 << 4));
    container->push_back(value % (1 << 4));

};


}
