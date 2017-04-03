#include "converter.h"


namespace conv {

    //
    // "convert" function group converts the variable type to 'unsigned char'
    // std::vector or 'unsigned char' to 'bool' std::vector in binary units.
    //


    // unsigned int -> unsigned char std::vector
    void convert (
            unsigned int origin, std::vector<unsigned char> *container) {

        // delete container content
        container->clear();

        // store 1 byte at a time
        for (int i = 0; i < static_cast<int>(sizeof(origin)); i++) {
            container->push_back(origin >> i * BIT_PER_BYTE);
        }
    }


    // unsigned short -> unsigned char std::vector
    void convert (
            unsigned short origin, std::vector<unsigned char> *container) {

        // delete container content
        container->clear();

        // store 1 byte at a time
        for (int i = 0; i < static_cast<int>(sizeof(origin)); i++) {
            container->push_back(origin >> i * BIT_PER_BYTE);
        }
    }


    // int -> unsigned char std::vector
    void convert (int origin, std::vector<unsigned char> *container) {

        // delete container content
        container->clear();

        // store 1 byte at a time
        for (int i = 0; i < static_cast<int>(sizeof(origin)); i++) {
            container->push_back(origin >> i * BIT_PER_BYTE);
        }
    }


    // short -> unsigned char std::vector
    void convert (short origin, std::vector<unsigned char> *container) {

        // delete container content
        container->clear();

        // store 1 byte at a time
        for (int i = 0; i < static_cast<int>(sizeof(origin)); i++) {
            container->push_back(origin >> i * BIT_PER_BYTE);
        }
    }


    // unsigned char -> bool std::vector
    void convert (unsigned char origin, std::vector<bool> *container) {

        // delete container content
        container->clear();

        // store 1 bit at a time
        for (int i = 0; i < BIT_PER_BYTE; i++) {
            container->push_back(origin >> i);
        }
    }


    // ===== ===== ===== ===== ===== ===== ===== ===== ===== =====


    //
    // dump function group converts "unsigned char" std::vector to
    // variable or in binary units.
    // "begin" argument represents the starting position of the std::vector.
    //


    // unsigned char std::vector -> unsigned int
    unsigned int dumpUInt (
            const std::vector<unsigned char> &origin, int begin) {

        // store binary units to the variable
        unsigned int value = origin[begin] |
            origin[begin + 1] << BIT_PER_BYTE |
            origin[begin + 2] << BIT_PER_BYTE * 2 |
            origin[begin + 3] << BIT_PER_BYTE * 3;

        return value;
    }


    // unsigned char std::vector -> int
    int dumpInt (const std::vector<unsigned char> &origin, int begin) {

        // store binary units to the variable
        int value = origin[begin] |
            origin[begin + 1] << BIT_PER_BYTE |
            origin[begin + 2] << BIT_PER_BYTE * 2 |
            origin[begin + 3] << BIT_PER_BYTE * 3;

        return value;
    }


    // unsigned short std::vector -> unsigned int
    unsigned short dumpUShort (
            const std::vector<unsigned char> &origin, int begin) {

        // store binary units to the variable
        unsigned short value = origin[begin] |
            origin[begin + 1] << BIT_PER_BYTE;

        return value;
    }


    // short std::vector -> unsigned int
    short dumpShort (
            const std::vector<unsigned char> &origin, int begin) {

        // store binary units to the variable
        short value = origin[begin] |
            origin[begin + 1] << BIT_PER_BYTE;

        return value;
    }

    // bool std::vector -> unsigned char
    unsigned char dumpUChar (
            const std::vector<bool> &origin, int begin) {

        // store binary units to the variable
        unsigned char value = 0;
        for (int i = 0; i < BIT_PER_BYTE; i++) {
            value += origin[begin + i] << i;
        }

        return value;
    }

}

