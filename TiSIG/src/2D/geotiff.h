#include <iostream>
#include <gdal/gdal_priv.h>
#include <pqxx/binarystring>
#include "../outils/dbmanager.h"
#include <filesystem>
#include <cmath>
#include <variant>
#include <iomanip>
#include <algorithm>

class Geotiff
{
private:
    std::string fileName;
    GDALDataset *poDataset;
    double minX;
    double minY;
    double maxX;
    double maxY;
    double pixelWidthResolution;
    double pixelHeightResolution;
    int bandCount;
    int width;
    int height;
    std::string filePath;

public:
    /**
     * @brief Constructs a Geotiff object with the specified file path.
     *
     * @param filePath The path of the Geotiff file.
     */
    Geotiff(std::string filePath);
    /**
     * @brief Destructor for the Geotiff class.
     *
     * This destructor closes the GDAL dataset associated with the Geotiff object.
     */
    ~Geotiff();
    /**
     * @brief Reads a GeoTIFF file.
     *
     * This function opens a GeoTIFF file in read-only mode using the GDAL library.
     * If the file cannot be opened, an error message is printed to the standard error stream.
     * If the file is opened successfully, a success message is printed to the standard output stream.
     */
    void Read();
    /**
     * Calculates the number of bands in the GeoTIFF dataset.
     *
     * @return The number of bands in the GeoTIFF dataset.
     */
    int CalculateNumberBands();
    /**
     * @brief Calculates the extent of the GeoTIFF.
     *
     * This function calculates the extent of the GeoTIFF by retrieving the geotransform
     * information from the dataset. It returns a vector of four double values representing
     * the minimum X, minimum Y, maximum X, and maximum Y coordinates of the extent.
     *
     * @return A vector of four double values representing the extent of the GeoTIFF.
     */
    std::vector<double> CalculateExtent();
    /**
     * @brief Calculates the resolution of the Geotiff.
     *
     * This function retrieves the geotransform of the Geotiff dataset and calculates the pixel width and height resolution.
     * The resolution values are rounded to 5 decimal places.
     *
     * @return A vector containing the pixel width and height resolution.
     */
    std::vector<double> CalculateResolution();
    /**
     * @brief Calculates the image dimensions of a GeoTIFF.
     *
     * This function calculates the width and height of the GeoTIFF image.
     *
     * @return A vector containing the width and height of the image.
     */
    std::vector<int> CalculateImageDimensions();
    /**
     * @brief Retrieves the file path of the Geotiff.
     *
     * @return The file path as a std::string.
     * @throws std::runtime_error if the fileName is not initialized or if the path does not exist.
     */
    std::string GetFilePath();
    /**
     * @brief Calculates the origin of the Geotiff.
     *
     * This function calculates the origin of the Geotiff by calling the CalculateExtent() function
     * and returns a vector containing the minimum X and maximum Y values.
     *
     * @return A vector containing the minimum X and maximum Y values.
     */
    std::vector<double> CalculateOrigin();
    /**
     * @brief Writes the GeoTIFF data and metadata to a PostGIS database.
     *
     * This function writes the raster data and associated metadata to a PostGIS database table.
     * It first checks if the GeoTIFF file is opened, and if not, it prints an error message and returns.
     * It then calculates the image dimensions, number of bands, and extent.
     * Next, it retrieves the file path and reads the raster data for each band.
     * The raster data is stored in a binary string and appended to the existing binary string.
     * Finally, it creates a table in the database if it doesn't exist and inserts the raster data and metadata into the table.
     *
     * @param db The DbManager object representing the connection to the PostGIS database.
     */
    void WriteGeotiffAndMetadataToPostgis(DbManager &db);
    /**
     * @brief Converts pixel coordinates to geographic coordinates.
     *
     * @param pixelX The X-coordinate of the pixel.
     * @param pixelY The Y-coordinate of the pixel.
     * @return std::vector<double> The geographic coordinates corresponding to the pixel.
     */
    std::vector<double> PixelToGeoCoordinates(int pixelX, int pixelY);
    /**
     * @brief Returns the number of images stored in the database.
     *
     * @param db The database manager object.
     * @return The number of images stored in the database.
     */
    int GetNumberOfImagesStored(DbManager &db);
    /**
     * @brief Returns the pixel value at the specified pixel coordinates.
     *
     * @param pixelX The X-coordinate of the pixel.
     * @param pixelY The Y-coordinate of the pixel.
     * @return std::vector<float> The pixel value at the specified pixel coordinates.
     */
    std::vector<float> GetPixelValue(int pixelX, int pixelY);
    /**
     * @brief Returns the pixel value at the specified geographic coordinates.
     *
     * @param geoX The X-coordinate of the geographic coordinates.
     * @param geoY The Y-coordinate of the geographic coordinates.
     * @return std::vector<float> The pixel value at the specified geographic coordinates.
     */
    std::vector<int> GeoCoordinatesToPixel(double geoX, double geoY);
};
