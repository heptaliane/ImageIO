#ifndef IMGUTILS_H_
#define IMGUTILS_H_

#include <vector>

#include "canvas.h"



namespace imgio {


// detect most proper color from palette
unsigned char allotPalette (
        const SimpleColor &color, const std::vector<SimpleColor> &palette);


// 1 bit color data
unsigned char collectColor1bit (const std::vector<unsigned char> &binary);


void convertColor1bit (
        unsigned char value, std::vector<unsigned char> *container);


// 4 bit color data
unsigned char collectColor4bit (const std::vector<unsigned char> &binary);


void convertColor4bit (
        unsigned char value, std::vector<unsigned char> *container);


}

#endif /* IMGUTILS_H_ */
