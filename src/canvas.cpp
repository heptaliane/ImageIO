#include "canvas.h"


namespace imgio {

//
// Canvas
//
// 2D SimpleColor matrix like image canvas
//

const SimpleColor Canvas::except = SimpleColor();

// constructor which initialized by Canvas size and color
Canvas::Canvas (unsigned int imgWidth, unsigned int imgHeight,
        const SimpleColor &color) :
    width(imgWidth), height(imgHeight),
    mat(new SimpleColor[imgWidth * imgHeight]) {

    int i, j;

    // set initial value to matrix
    for ( i = 0; i < static_cast<int>(imgWidth); i++ ) {
        for ( j = 0; j < static_cast<int>(imgHeight); j++ ) {
            mat[i * imgHeight + j] = color;
        }
    }

}


// copy constructor
Canvas::Canvas (const Canvas &obj) : mat(NULL) {

    copy (obj);
}

// destructor
Canvas::~Canvas () {

    delete [] mat;
    mat = NULL;

}


// get value from matrix
const SimpleColor &Canvas::get (unsigned int row, unsigned int col) const {

    if (isIndexInRange(row, col)) {
        return mat[row * width + col];
    }
    // if index is out of range, return exceptional value
    return except;

}


// get first pointer of matrix
const unsigned char* Canvas::getPointer () const {

    return reinterpret_cast<unsigned char*>(mat);
}

unsigned char* Canvas::getPointer () {

    return reinterpret_cast<unsigned char*>(mat);
}


// set value to matrix
void Canvas::set (unsigned int row, unsigned int col,
        const SimpleColor &color) {

    if (isIndexInRange(row, col)) {
        mat[row * width + col] = color;
    }

}


// resize matrix
void Canvas::resize (unsigned int newWidth, unsigned int newHeight) {

    // load size
    width = newWidth;
    height = newHeight;

    // update matrix
    delete[] mat;
    mat = new SimpleColor[newWidth * newHeight];

}

void Canvas::copy (const Canvas &obj) {

    // adjust to original size
    resize(obj.cols(), obj.rows());

    int i, j;

    // copy matrix
    for ( i = 0; i < static_cast<int>(height); i++ ) {
        for ( j = 0; j < static_cast<int>(width); j++ ) {
            set(i, j, obj.get(i, j));
        }
    }

}



// ===== ===== ===== ===== ===== ===== ===== ===== ===== =====

}
