cmake_minimum_required(VERSION 3.12)
project(ImportRaster)

find_package(OpenCV REQUIRED)

add_executable(import_raster import_raster.cpp)

# Add libcurl to the target_link_libraries
target_link_libraries(import_raster PRIVATE curl ${OpenCV_LIBS})
