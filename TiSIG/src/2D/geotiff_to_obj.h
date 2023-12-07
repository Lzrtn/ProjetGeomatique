#include "gdal/gdal_priv.h"
#include <stdexcept>
#include <iostream>
#include <string.h>
class GeoTiffToObjConverter {
public:
    GeoTiffToObjConverter(const std::string inputFilePath,const std::string inputOrthoFilePath, const std::string outputObjFilePath);
    GeoTiffToObjConverter(const std::string inputFilePath, const std::string outputObjFilePath);
    ~GeoTiffToObjConverter();

    void writeObjFileWithTextures();
    void writeObjFileWithoutTextures();
    // void writeObjFiles();

private:
    const std::string inputFilePath;
    const std::string inputOrthoFilePath;
    const std::string outputObjFilePath;
    GDALDataset* dataset;
    GDALDataset* orthoDataset;
};
