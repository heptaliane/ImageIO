cmake_minimum_required (VERSION 3.6)

SET (CMAKE_C_FLAGS_RELEASE "-Wall -O2")
SET (CMAKE_BUILD_RELEASE "-g")
SET (CMAKE_BUILD_TYPE Release)

set (bmploader ${CMAKE_SOURCE_DIR}/libbmploader.so)


add_executable (clone.out ../sample/01.cpp)
target_link_libraries (clone.out ${bmploader})

add_executable (gray.out ../sample/02.cpp)
target_link_libraries (gray.out ${bmploader})

