#ifndef GEOJSON_H
#define GEOJSON_H
#include <iostream>
#include <gdal/gdal_priv.h>
#include <string>

class GeoJson
{
public:
    /**
     * @brief Constructs a GeoJson object with the given file path.
     *
     * @param filePath The path to the GeoJSON file.
     */
    GeoJson(const std::string &filePath);
    /**
     * @brief Destructor for the GeoJson class.
     *
     * This destructor closes all drivers and performs necessary cleanup operations.
     */
    ~GeoJson();
    /**
     * @brief Converts a GeoJSON file to an ESRI Shapefile.
     *
     * This function opens the specified GeoJSON file, converts it to an ESRI Shapefile,
     * and saves the converted file to the specified output path.
     *
     * @note This function requires the GDAL library to be installed.
     *
     * @param filePath The path to the input GeoJSON file.
     * @param shpFilePath The path to save the converted ESRI Shapefile.
     */
    void ConvertToShp();
    /**
     * @brief Get the file path of the Shapefile.
     *
     * @return std::string The file path of the Shapefile.
     */
    std::string GetShpFilePath() { return shpFilePath; };

private:
    std::string filePath;    // Path of the GeoJSON file
    std::string shpFilePath; // Path of the SHP file
};

#endif // GEOJSON_H