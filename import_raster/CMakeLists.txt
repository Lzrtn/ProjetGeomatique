cmake_minimum_required(VERSION 3.12)
project(ImportRaster)

find_package(OpenCV REQUIRED)

add_executable(main main.cpp wms.h wms.cpp wmts.h wmts.cpp)

# Add libcurl to the target_link_libraries
target_link_libraries(main PRIVATE curl ${OpenCV_LIBS})
