
#include "bmp/dealer.h"

#include "imageio.h"
#include "canvas.h"

#include <fstream>
#include <vector>
#include <iostream>

namespace imgio {

    imgio::Canvas imread(std::string filename) {
        std::ifstream fin;
        unsigned char uc;
        std::vector<unsigned char> bin;
        fin.open(filename.c_str(), std::ios::binary);

        if (!fin) {
            std::cout << "ImageIO > cannnot load file : " << filename
                      << std::endl;
            return imgio::Canvas();
        }

        while (!fin.eof()) {
            fin.read(reinterpret_cast<char*>(&uc), sizeof(uc));
            bin.push_back(uc);
        }

        fin.close();
        imgio::Canvas img;

        int dot_pos = filename.find_last_of(".");
        std::string file_ext = 
                filename.substr(dot_pos, filename.size()-dot_pos);

        // BMP loader
        if (file_ext == ".bmp" || file_ext == ".BMP") {
          imgio::BmpDealer dealer(bin);

          if (!dealer.check(bin)) {
              std::cout << "ImageIO > Invalid file : " << filename
                        << std::endl;
          }
          dealer.getImage(&img);
        }
        else {
            std::cout << "ImageIO > Unsuported filetype : " << file_ext
                      << std::endl;
        }

        return img;
    }

    bool imwrite(std::string filename, imgio::Canvas img) {
        if (img.isEmpty()) {
            std::cout << "ImageIO > Canvas is empty" << std::endl;
            return false;
        }
        std::ofstream fout;
        fout.open(filename);
        std::vector<unsigned char> bin;

        int dot_pos = filename.find_last_of(".");
        std::string file_ext = 
                filename.substr(dot_pos, filename.size()-dot_pos);

        // BMP Save
        if (file_ext == ".bmp" || file_ext == ".BMP") {
            imgio::BmpDealer dealer;
            dealer.setImage(img);
            dealer.getBinary(&bin);
        }
        else {
            std::cout << "ImageIO > Unsuported filetype : " << file_ext
                      << std::endl;
            return false;
        }

        for (int k = 0; k < bin.size(); k++ ) {
            fout.write(reinterpret_cast<char*>(&bin[k]), sizeof(bin[k]));
        }
        fout.close();
        return true;
    }
}

