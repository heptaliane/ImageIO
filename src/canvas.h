#ifndef CANVAS_H_
#define CANVAS_H_

#include <cstddef>


namespace imgio {


//
// SimpleColor is a structure which holds only RGB values.
// the order of the values is red blue green
//

struct SimpleColor {


    // default constructor
    SimpleColor () :
        red(0), green(0), blue(0) {};


    // initialized by RGB values
    SimpleColor (unsigned char r,
            unsigned char g,
            unsigned char b) :
        red(r), green(g), blue(b) {};


    // copy constructor
    SimpleColor (const SimpleColor &obj) :
        red(obj.red), green(obj.green), blue(obj.blue) {};


    // entity
    unsigned char red;
    unsigned char green;
    unsigned char blue;
};


// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====


//
// 2D SimpleColor matrix like image canvas
//

class Canvas {
public:


    // default constructor
    Canvas ():
        width(0), height(0), mat(NULL) {};


    // initialized by Canvas size (width and height)
    Canvas (unsigned int imgWidth, unsigned int imgHeight) :
        width(imgWidth),
        height(imgHeight),
        mat(new SimpleColor[width * height]) {};


    // initialized by Canvas size and color
    Canvas (unsigned int imgWidth, unsigned int imgHeight,
            const SimpleColor &color);


    // copy constructor
    Canvas (const Canvas &obj);


    // destructor
    ~Canvas ();


    // get value from matrix
    const SimpleColor &get (unsigned int row, unsigned int col) const;


    // get first pointer of matrix
    const unsigned char* getPointer () const;


    // set value to matrix
    void set (unsigned int row, unsigned int col,
            const SimpleColor &color);


    // resize the matrix
    void resize (unsigned int newWidth, unsigned int newHeight);


    // copy matrix
    void copy (const Canvas &obj);


    // get matrix size
    unsigned int rows () const {
        return height;
    };

    unsigned int cols () const {
        return width;
    };



private:


    // constant
    static const SimpleColor except;


    // check if the coordinate is out of range or not
    bool isIndexInRange (unsigned int row, unsigned int col) const {
        return ( row < height && col < width );
    };


    // entity
    unsigned int width;
    unsigned int height;
    SimpleColor *mat;

};


}

#endif /* CANVAS_H_ */
