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



std::tuple<std::string, double, double> GeoTiffToObjConverter::writeObjFileWithTextures()
{
    double adfGeoTransform[6];
    if (GDALGetGeoTransform(dataset, adfGeoTransform) == CE_None)
    {
        size_t lastSlash = inputFilePath.find_last_of("/\\");
        std::string fileName = inputFilePath.substr(lastSlash + 1);
        std::string baseName = fileName.substr(0, fileName.find_last_of("."));

        int width = GDALGetRasterXSize(dataset);
        int height = GDALGetRasterYSize(dataset);
        int subWidth = width / 3;
        int subHeight = height / 2;
        int orthoWidth = GDALGetRasterXSize(orthoDataset);
        int orthoHeight = GDALGetRasterYSize(orthoDataset);

        // Save the current locale
        const char *currentLocale = setlocale(LC_NUMERIC, nullptr);

        // Set the "C" locale to ensure dots as decimal separators
        setlocale(LC_NUMERIC, "C");

        for (int i = 0; i < 6; ++i)
        {
            std::string strOutputObjFilePath = outputObjFilePath + baseName + "_" + std::to_string(i) + ".obj";
            FILE *objFile = fopen(strOutputObjFilePath.c_str(), "w");
            if (!objFile)
            {
                throw std::runtime_error("Unable to create the .obj file");
            }

            int startX = (i % 3) * subWidth;
            int startY = (i / 3) * subHeight;
            int currentSubWidth = subWidth + 1;
            int currentSubHeight = subHeight + 1;

            if (i % 3 == 2)
            {
                currentSubWidth = width - startX;
            }

            if (i / 3 == 1)
            {
                currentSubHeight = height - startY;
            }


            for (int y = startY; y < startY + currentSubHeight; ++y)
            {
                for (int x = startX; x < startX + currentSubWidth; ++x)
                {
                    double value;
                    CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
                    if (err != CE_None)
                    {
                        throw std::runtime_error("Error reading raster data");
                    }

                    double x_geo = (x * adfGeoTransform[1] + y * adfGeoTransform[2]);
                    double y_geo = -(x * adfGeoTransform[4] + y * adfGeoTransform[5]);
                    fprintf(objFile, "v %f %f %f\n", x_geo, y_geo, value);

                    float texCoordX = (float)x * (orthoWidth - 1) / (width - 1);
                    float texCoordY = (float)y * (orthoHeight - 1) / (height - 1);

                    fprintf(objFile, "vt %f %f\n", texCoordX / (orthoWidth - 1), texCoordY / (orthoHeight - 1));

                    fprintf(objFile, "vn 0 0 1\n");
                }
            }

            for (int y = startY; y < startY + currentSubHeight - 1; ++y)
            {
                for (int x = startX; x < startX + currentSubWidth - 1; ++x)
                {
                    int v1 = ((y - startY) * currentSubWidth + (x - startX)) + 1;
                    int v2 = ((y - startY) * currentSubWidth + (x - startX + 1)) + 1;
                    int v3 = (((y - startY + 1) * currentSubWidth) + (x - startX + 1)) + 1;
                    int v4 = (((y - startY + 1) * currentSubWidth) + (x - startX)) + 1;

                    fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v3, v3, v3, v2, v2, v2);
                    fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v4, v4, v4, v3, v3, v3);
                }
            }

            fclose(objFile);
        }

        setlocale(LC_NUMERIC, currentLocale);
        double xTranslate = adfGeoTransform[0];
        double yTranslate = adfGeoTransform[3];
        return std::make_tuple(baseName, xTranslate, yTranslate);
        ///pour romain
        //std::tuple<std::string, double, double> result = geoTiffConverter.writeObjFileWithTextures();
        // std::string baseName = std::get<0>(result);
        // double xTranslate = std::get<1>(result);
        // double yTranslate = std::get<2>(result);
    }
    else
    {
        throw std::runtime_error("Error getting geotransform information");
    }
}
