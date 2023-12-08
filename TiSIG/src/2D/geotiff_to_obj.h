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

/**
 * @brief The GeoTiffToObjConverter class is responsible for converting a GeoTIFF file to an OBJ file with textures.
 * 
 * This class provides methods to read a GeoTIFF file, an orthophoto file, and write an OBJ file with textures.
 * The class takes the input file paths for the GeoTIFF file, orthophoto file, and the output OBJ file as parameters.
 * It also provides a method to write the OBJ file with textures, which returns a tuple containing the file path of the OBJ file
 * and the geotiff origin coordinates.
 */
class GeoTiffToObjConverter {
public:
    /**
     * @brief Constructs a GeoTiffToObjConverter object.
     *
     * @param inputFilePath The path to the input GeoTIFF file.
     * @param inputOrthoFilePath The path to the input orthophoto file.
     * @param outputObjFilePath The path to the output OBJ file.
     */
    GeoTiffToObjConverter(const std::string inputFilePath,const std::string inputOrthoFilePath, const std::string outputObjFilePath);

    /**
     * @brief Destructor for the GeoTiffToObjConverter class.
     */
    ~GeoTiffToObjConverter();

    /**
     * @brief Represents a tuple containing a string, a double value, and another double value.
     * 
     * This tuple is used to store information related to writing an OBJ file with textures.
     * The first element of the tuple is a string representing the file path of the OBJ file.
     * The second and third elements are double values representing geotiff origin coordinates.
     * 
     * @return A tuple containing a string, a double value, and another double value representing geotiff origin coordinates.
     */
    std::tuple<std::string, double, double> writeObjFileWithTextures();


private:
    const std::string inputFilePath;        // The path to the input GeoTIFF file.
    const std::string inputOrthoFilePath;   // The path to the input orthophoto file.
    const std::string outputObjFilePath;    // The path to the output OBJ file.
    GDALDataset* dataset;                    // Pointer to the GDALDataset representing the DTM GeoTIFF file.
    GDALDataset* orthoDataset;               // Pointer to the GDALDataset representing the orthophoto file.
};
