#ifndef CONVERTER_H_
#define CONVERTER_H_

#include <vector>


// 1 byte is 8 bit
#define BIT_PER_BYTE 8


namespace conv {

    //
    // "convert" function group converts the variable type to 'unsigned char'
    // std::vector or 'unsigned char' to 'bool' std::vector in binary units.
    //


    // unsigned int -> unsigned char std::vector
    void convert (unsigned int origin, std::vector<unsigned char> *container);


    // unsigned short -> unsigned char std::vector
    void convert (
            unsigned short origin, std::vector<unsigned char> *container);


    // int -> unsigned char std::vector
    void convert (int origin, std::vector<unsigned char> *container);


    // short -> unsigned char std::vector
    void convert (short origin, std::vector<unsigned char> *container);


    // unsigned char -> bool std::vector
    void convert (unsigned char origin, std::vector<bool> *container);



    // ===== ===== ===== ===== ===== ===== ===== ===== ===== =====


    //
    // dump function group converts "unsigned char" std::vector to
    // variable or in binary units.
    // "begin" argument represents the starting position of the std::vector.
    //


    // unsigned char std::vector -> unsigned int
    unsigned int dumpUInt (
            const std::vector<unsigned char> &origin, int begin = 0);


    // unsigned char std::vector -> int
    int dumpInt (const std::vector<unsigned char> &origin, int begin = 0);


    // unsigned short std::vector -> unsigned int
    unsigned short dumpUShort (
            const std::vector<unsigned char> &origin, int begin = 0);


    // short std::vector -> unsigned int
    short dumpShort (
            const std::vector<unsigned char> &origin, int begin = 0);


    // bool std::vector -> unsigned char
    unsigned char dumpUChar (const std::vector<bool> &origin, int begin = 0);


}


#endif /* CONVERTER_H_ */
