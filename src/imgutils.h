#ifndef IMGUTILS_H_
#define IMGUTILS_H_

#include <vector>
#include "canvas.h"

namespace {

//
// square the value
//
int square (int value) {
    return value * value;
}

}


// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====



namespace imgio {


//
// detect most proper color from palette
//
unsigned char allotPalette (
        const SimpleColor &color, const std::vector<SimpleColor> &palette) {

    // minimum variance palette index ( initialized with 0 )
    unsigned char pidx = 0;

    // minimum variance ( initialized with maximum value )
    int minvar = 1 << 18;
    int var;

    // check with all color in the palette
    for ( int i = 0; i < palette.size(); i++ ) {

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

    }

    return pidx;

}


}

#endif /* IMGUTILS_H_ */
