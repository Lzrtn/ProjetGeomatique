#include "gdal/gdal_priv.h"
#include <stdexcept>
#include <iostream>
#include <string.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <vector>
#include <string>

class GeoTiffToObjConverter {
public:
    GeoTiffToObjConverter(const std::string inputFilePath,const std::string inputOrthoFilePath, const std::string outputObjFilePath);
    ~GeoTiffToObjConverter();

    std::string writeObjFileWithTextures();


private:
    const std::string inputFilePath;
    const std::string inputOrthoFilePath;
    const std::string outputObjFilePath;
    GDALDataset* dataset;
    GDALDataset* orthoDataset;
};
