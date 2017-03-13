cmake_minimum_required (VERSION 3.6)

SET (CMAKE_C_FLAGS_RELEASE "-Wall -O2")
SET (CMAKE_BUILD_RELEASE "-g")
SET (CMAKE_BUILD_TYPE Release)

set (bmploader ${CMAKE_SOURCE_DIR}/libbmploader.so)


add_executable (clone.out 01.cpp)
target_link_libraries (clone.out ${bmploader})

add_executable (gray.out 02.cpp)
target_link_libraries (gray.out ${bmploader})

add_executable (clone2.out 03.cpp)
target_link_libraries (clone2.out ${bmploader})
