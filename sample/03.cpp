
#include "../src/imageio.h"

int main (int argc, char **argv) {

    // load
    imgio::Canvas img = imgio::imread(argv[1]);

    // save
    imgio::imwrite(argv[2], img);
}
