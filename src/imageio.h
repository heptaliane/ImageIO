
#ifdef IMAGEIO_H_20170313
#define IMAGEIO_H_20170313

#include "bmp/dealer.h"
#include "canvas.h"

#include <string>

namespace imgio {
  Canvas imread(std::string filename);
}

#endif
