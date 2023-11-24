#include <iostream>
#include <gdal/gdal_priv.h>
#include <pqxx/binarystring>
#include "../outils/dbmanager.h"
#include <filesystem>
#include <cmath>
#include <variant>
#include <iomanip>
#include <algorithm>


class Geotiff {
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
        Geotiff(std::string fileName);
        ~Geotiff();
        void Read();
        int CalculateNumberBands();
        std::vector<double> CalculateExtent(); // Provide template argument for std::vector
        std::vector<double> CalculateResolution();
        std::vector<int> CalculateImageDimensions();
        std::string GetFilePath();
        std::vector<double> CalculateOrigin();
        void WriteGeotiffAndMetadataToPostgis(DbManager& db);
        std::vector<double> PixelToGeoCoordinates(int pixelX, int pixelY);
        int GetNumberOfImagesStored(DbManager& db);
        std::vector<float> GetPixelValue(int pixelX, int pixelY);
        std::vector<int> GeoCoordinatesToPixel(double geoX, double geoY);
};
