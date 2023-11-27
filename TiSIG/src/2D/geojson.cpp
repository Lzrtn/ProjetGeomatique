#include "geojson.h"

GeoJson::GeoJson(const std::string &filePath) : filePath(filePath)
{
    // Extract the filename without extension
    size_t lastSlash = filePath.find_last_of("/\\");
    std::string fileNameWithoutExt = filePath.substr(lastSlash + 1, filePath.find_last_of(".") - lastSlash - 1);

    // Extract the directory
    std::string directory = filePath.substr(0, lastSlash + 1);

    // Set the SHP file path
    shpFilePath = directory + fileNameWithoutExt + ".shp";

    // Initialize the GDAL drivers for GeoJSON and SHP files
    GDALAllRegister();
}

GeoJson::~GeoJson()
{
    GDALDestroyDriverManager();
}

void GeoJson::ConvertToShp()
{

    GDALDataset *poDS = (GDALDataset *)GDALOpenEx(filePath.c_str(), GDAL_OF_VECTOR, NULL, NULL, NULL);

    if (poDS == NULL)
    {
        std::cout << "Could not open the GeoJSON file" << std::endl;
        return;
    }

    GDALDriver *poDriver = GetGDALDriverManager()->GetDriverByName("ESRI Shapefile");

    GDALDataset *poDSOut = poDriver->CreateCopy(shpFilePath.c_str(), poDS, FALSE, NULL, NULL, NULL);

    GDALClose(poDS);
    GDALClose(poDSOut);
}
