CMAKE_MINIMUM_REQUIRED (VERSION 3.6)

SET (CMAKE_C_FLAGS_RELEASE "-Wall -O2")
SET (CMAKE_C_FLAGS_DEBUG "-g")

SET (CMAKE_BUILD_TYPE Release)

PROJECT (ImageIO CXX)
SET (serial "1.0.0")
SET (soserial "1")

FILE (GLOB common src/*.cpp)
FILE (GLOB commonheader src/*.h)
FILE (GLOB bmp src/bmp/*.cpp)
FILE (GLOB bmpheader src/bmp/*.h)

INCLUDE_DIRECTORIES (
    src/
    src/bmp/
)

ADD_LIBRARY (
    bmploader SHARED
    ${common}
    ${bmp}
)
SET_TARGET_PROPERTIES (
    bmploader PROPERTIES
    VERSION ${serial}
    SOVERSION ${soserial}
)

INSTALL (
    FILES ${commonheader} ${bmpheader}
    DESTINATION include
)
INSTALL (
    TARGETS bmploader
    LIBRARY DESTINATION lib
)
