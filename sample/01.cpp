#include <vector>
#include <fstream>

#include "../src/canvas.h"
#include "../src/bmp/dealer.h"


void loadBinary (const char *file, std::vector<unsigned char> *container) {

    std::ifstream fin;
    unsigned char uc;

    // load file
    fin.open(file, std::ios::binary);

    // no file found
    if (!fin) {
        return;
    }

    // store data
    while (!fin.eof()) {
        fin.read(reinterpret_cast<char*>(&uc), sizeof(uc));
        container->push_back(uc);
    }
    fin.close();

}


void dumpBinary (const char *file, const std::vector<unsigned char> &binary) {

    std::ofstream fout;
    std::vector<unsigned char> buffer(binary.begin(), binary.end());

    // dump file
    fout.open(file);


    for ( int k = 0; k < buffer.size(); k++ ) {
        fout.write(
                reinterpret_cast<char*>(&buffer[k]), sizeof(buffer[k]));
    }
}


int main (int argc, char **argv) {

    std::vector<unsigned char> binary;

    loadBinary(argv[1], &binary);

    imgio::BmpDealer dealer;

    // load bitmap data
    dealer.loadBinary(binary);

    // export image data
    imgio::Canvas image;
    dealer.getImage(&image);

    // dump binary data
    dealer.getBinary(&binary);

    // write file
    dumpBinary(argv[2], binary);
}
