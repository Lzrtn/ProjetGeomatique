#include "geotiff_to_obj.h"

GeoTiffToObjConverter::GeoTiffToObjConverter(const std::string inputFilePath, const std::string inputOrthoFilePath, const std::string outputObjFilePath)
    : inputFilePath(inputFilePath), inputOrthoFilePath(inputOrthoFilePath), outputObjFilePath(outputObjFilePath), dataset(nullptr), orthoDataset(nullptr)
{

    GDALAllRegister();
    dataset = (GDALDataset *)GDALOpen(inputFilePath.c_str(), GA_ReadOnly);
    orthoDataset = (GDALDataset *)GDALOpen(inputOrthoFilePath.c_str(), GA_ReadOnly);

    if (!dataset)
    {
        throw std::runtime_error("Unable to open the GeoTIFF file");
    }
    if (!orthoDataset)
    {
        throw std::runtime_error("Unable to open the orthophoto GeoTIFF file");
    }
}



GeoTiffToObjConverter::~GeoTiffToObjConverter()
{
    if (dataset)
    {
        GDALClose(dataset);
    }
    if (orthoDataset)
    {
        GDALClose(orthoDataset);
    }
}

std::string GeoTiffToObjConverter::writeObjFileWithTextures()
{
    double adfGeoTransform[6];
    if (GDALGetGeoTransform(dataset, adfGeoTransform) == CE_None)
    {
        size_t lastSlash = inputFilePath.find_last_of("/\\");
        std::string fileName = inputFilePath.substr(lastSlash + 1);
        std::string baseName = fileName.substr(0, fileName.find_last_of("."));

        int width = GDALGetRasterXSize(dataset);
        int height = GDALGetRasterYSize(dataset);
        int subWidth = width / 4;
        int subHeight = height / 2;
        int orthoWidth = GDALGetRasterXSize(orthoDataset);
        int orthoHeight = GDALGetRasterYSize(orthoDataset);
        // Save the current locale
        const char *currentLocale = setlocale(LC_NUMERIC, nullptr);

        // Set the "C" locale to ensure dots as decimal separators
        setlocale(LC_NUMERIC, "C");
        int a = 0;
        double x_translate, y_translate;
        for (int i = 0; i < 8; ++i)
        {
            std::string strOutputObjFilePath = outputObjFilePath + baseName + "_" + std::to_string(i) + ".obj";
            FILE *objFile = fopen(strOutputObjFilePath.c_str(), "w");
            if (!objFile)
            {
                throw std::runtime_error("Unable to create the .obj file");
            }

            int startX = (i % 4) * subWidth;
            int startY = (i / 4) * subHeight;
            std::cout << "image " << i << " " << startX << " " << startY << std::endl;

            for (int y = startY; y < startY + subHeight; ++y)
            {
                for (int x = startX; x < startX + subWidth; ++x)
                {
                    double value;
                    CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
                    if (err != CE_None)
                    {
                        throw std::runtime_error("Error reading raster data");
                    }

                    if (a == 0)
                    {
                        x_translate = (adfGeoTransform[0] + x * adfGeoTransform[1] + y * adfGeoTransform[2]);
                        y_translate = (adfGeoTransform[3] + x * adfGeoTransform[4] + y * adfGeoTransform[5]);
                        a = 1;
                    }

                    double x_geo = (adfGeoTransform[0] + x * adfGeoTransform[1] + y * adfGeoTransform[2]) - x_translate;
                    double y_geo = -(adfGeoTransform[3] + x * adfGeoTransform[4] + y * adfGeoTransform[5]) + y_translate;
                    fprintf(objFile, "v %f %f %f\n", x_geo, y_geo, value);

                    float texCoordX = (float)x * (orthoWidth - 1) / (width - 1);
                    float texCoordY = (float)y * (orthoHeight - 1) / (height - 1);

                    fprintf(objFile, "vt %f %f\n", texCoordX / (orthoWidth - 1), texCoordY / (orthoHeight - 1));

  
                    fprintf(objFile, "vn 0 0 1\n");

                }
            }
            for (int y = startY; y < startY + subHeight - 1; ++y)
            {
                for (int x = startX; x < startX + subWidth - 1; ++x)
                {
                    // Rest of the code, with x and y adjusted by startX and startY respectively
                    int v1 = ((y - startY) * subWidth + (x - startX)) + 1; // Adjusted indices
                    int v2 = ((y - startY) * subWidth + (x - startX + 1)) + 1;
                    int v3 = (((y - startY + 1) * subWidth) + (x - startX + 1)) + 1;
                    int v4 = (((y - startY + 1) * subWidth) + (x - startX)) + 1;

                    fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v2, v2, v2, v3, v3, v3);
                    fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v3, v3, v3, v4, v4, v4);
                }
            }

            fclose(objFile);
            return baseName;
        }
    }
    else
    {
        throw std::runtime_error("Error getting geotransform information");
    }
    
}
