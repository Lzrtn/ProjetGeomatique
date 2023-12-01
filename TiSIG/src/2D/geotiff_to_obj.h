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
    GeoTiffToObjConverter(const std::string inputFilePath, const std::string outputObjFilePath);
    ~GeoTiffToObjConverter();

    void writeObjFileWithTextures();
    void writeCoordWithTextures();
    void writeObjFileWithoutTextures();
    // void writeObjFiles();

    std::vector<QVector3D> getPosition() { return m_position; };
    std::vector<QVector3D> getNormal(){return m_normal; };
    std::vector<QVector2D> getTexture(){return m_textCoord; };


private:
    const std::string inputFilePath;
    const std::string inputOrthoFilePath;
    const std::string outputObjFilePath;
    GDALDataset* dataset;
    GDALDataset* orthoDataset;

    std::vector<QVector3D> m_position;
    std::vector<QVector3D> m_normal;
    std::vector<QVector2D> m_textCoord;
};
