
#ifdef IMAGEIO_H_20170313
#define IMAGEIO_H_20170313

#include "bmp/dealer.h"
#include "canvas.h"

#include <string>

namespace imgio {
    imgio::Canvas imread(const std::string& filename);
    bool imwrite(const std::string& filename, const imgio::Canvas& img);
}

#endif
