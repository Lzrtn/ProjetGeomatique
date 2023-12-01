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

GeoTiffToObjConverter::GeoTiffToObjConverter(const std::string inputFilePath, const std::string outputObjFilePath)
    : inputFilePath(inputFilePath), outputObjFilePath(outputObjFilePath), dataset(nullptr), orthoDataset(nullptr)
{

    GDALAllRegister();
    dataset = (GDALDataset *)GDALOpen(inputFilePath.c_str(), GA_ReadOnly);

    if (!dataset)
    {
        throw std::runtime_error("Unable to open the GeoTIFF file");
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

void GeoTiffToObjConverter::writeObjFileWithTextures()
{
    size_t lastSlash = inputFilePath.find_last_of("/\\");
    std::string fileName = inputFilePath.substr(lastSlash + 1);
    std::cout << fileName << std::endl;
    std::string baseName = fileName.substr(0, fileName.find_last_of("."));
    std::cout << baseName << std::endl;
    std::string strOutputObjFilePath = outputObjFilePath + baseName + ".obj";

    FILE *objFile = fopen(strOutputObjFilePath.c_str(), "w");
    if (!objFile)
    {
        throw std::runtime_error("Unable to create the .obj file");
    }

    int width = GDALGetRasterXSize(dataset);
    int height = GDALGetRasterYSize(dataset);
    int orthoWidth = GDALGetRasterXSize(orthoDataset);
    int orthoHeight = GDALGetRasterYSize(orthoDataset);
    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double value;
            CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
            if (err != CE_None)
            {
                throw std::runtime_error("Error reading raster data");
            }

            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
        }
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double value;
            CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
            if (err != CE_None)
            {
                throw std::runtime_error("Error reading raster data");
            }

            double normalizedValue = ((value - minValue) / (maxValue - minValue));
            fprintf(objFile, "v %d %f %d\n", x, normalizedValue, y);
            float texCoordX = (float)x * (orthoWidth - 1) / (width - 1);
            float texCoordY = (float)y * (orthoHeight - 1) / (height - 1);

            fprintf(objFile, "vt %f %f\n", texCoordX / (orthoWidth - 1), texCoordY / (orthoHeight - 1));

            fprintf(objFile, "vn 0 1 0\n");
        }
    }
    // Write faces to the .obj file
    for (int y = 0; y < height - 1; ++y)
    {
        for (int x = 0; x < width - 1; ++x)
        {
            int v1 = y * width + x + 1;
            int v2 = y * width + x + 2;
            int v3 = (y + 1) * width + x + 2;
            int v4 = (y + 1) * width + x + 1;

            fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v2, v2, v2, v3, v3, v3);
            fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v3, v3, v3, v4, v4, v4);
        }
    }

    fclose(objFile);
}


void GeoTiffToObjConverter::writeCoordWithTextures()
{
//    size_t lastSlash = inputFilePath.find_last_of("/\\");
//    std::string fileName = inputFilePath.substr(lastSlash + 1);
//    std::cout << fileName << std::endl;
//    std::string baseName = fileName.substr(0, fileName.find_last_of("."));
//    std::cout << baseName << std::endl;
//    std::string strOutputObjFilePath = outputObjFilePath + baseName + ".obj";

//    FILE *objFile = fopen(strOutputObjFilePath.c_str(), "w");
//    if (!objFile)
//    {
//        throw std::runtime_error("Unable to create the .obj file");
//    }

    int width = GDALGetRasterXSize(dataset);
    int height = GDALGetRasterYSize(dataset);
    int orthoWidth = GDALGetRasterXSize(orthoDataset);
    int orthoHeight = GDALGetRasterYSize(orthoDataset);
    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();

    std::vector<QVector3D> position = {};
    std::vector<QVector3D> normal = {};
    std::vector<QVector2D> textCoord = {};

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double value;
            CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
            if (err != CE_None)
            {
                throw std::runtime_error("Error reading raster data");
            }

            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
        }
    }

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double value;
            CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
            if (err != CE_None)
            {
                throw std::runtime_error("Error reading raster data");
            }

            float normalizedValue = ((value - minValue) / (maxValue - minValue));
//            fprintf(objFile, "v %d %f %d\n", x, normalizedValue, y);
            float x_f = x;
            float y_f = y;
            position.push_back({x_f, normalizedValue, y_f});

            float texCoordX = (float)x * (orthoWidth - 1) / (width - 1);
            float texCoordY = (float)y * (orthoHeight - 1) / (height - 1);

//            fprintf(objFile, "vt %f %f\n", texCoordX / (orthoWidth - 1), texCoordY / (orthoHeight - 1));
            textCoord.push_back({ texCoordX / (orthoWidth - 1), texCoordY / (orthoHeight - 1)});

//            fprintf(objFile, "vn 0 1 0\n");
            normal.push_back({0, 1, 0});
        }
    }
//    // Write faces to the .obj file
//    for (int y = 0; y < height - 1; ++y)
//    {
//        for (int x = 0; x < width - 1; ++x)
//        {
//            int v1 = y * width + x + 1;
//            int v2 = y * width + x + 2;
//            int v3 = (y + 1) * width + x + 2;
//            int v4 = (y + 1) * width + x + 1;

//            fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v2, v2, v2, v3, v3, v3);
//            fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v3, v3, v3, v4, v4, v4);
//        }
//    }

//    fclose(objFile);
    m_position = position;
    m_normal = normal;
    m_textCoord = textCoord;
}

void GeoTiffToObjConverter::writeObjFileWithoutTextures()
{
    size_t lastSlash = inputFilePath.find_last_of("/\\");
    std::string fileName = inputFilePath.substr(lastSlash + 1);
    std::string baseName = fileName.substr(0, fileName.find_last_of("."));
    std::string strOutputObjFilePath = outputObjFilePath + baseName + ".obj";

    FILE *objFile = fopen(strOutputObjFilePath.c_str(), "w");
    if (!objFile)
    {
        throw std::runtime_error("Unable to create the .obj file");
    }
    if (!dataset)
    {
        throw std::runtime_error("Unable to open the GeoTIFF file");
    }
    int width = GDALGetRasterXSize(dataset);
    int height = GDALGetRasterYSize(dataset);
    double minValue = std::numeric_limits<double>::max();
    double maxValue = std::numeric_limits<double>::min();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double value;
            CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
            if (err != CE_None)
            {
                throw std::runtime_error("Error reading raster data");
            }

            minValue = std::min(minValue, value);
            maxValue = std::max(maxValue, value);
        }
    }
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            double value;
            CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
            if (err != CE_None)
            {
                throw std::runtime_error("Error reading raster data");
            }

            double normalizedValue = ((value - minValue) / (maxValue - minValue));
            fprintf(objFile, "v %d %f %d\n", x, normalizedValue, y);
            float texCoordX = (float)x / (width - 1);
            float texCoordY = (float)y / (height - 1);

            fprintf(objFile, "vt %f %f\n", texCoordX, texCoordY);

            fprintf(objFile, "vn 0 1 0\n");
        }
    }
    // Write faces to the .obj file (simple grid)
    for (int y = 0; y < height - 1; ++y)
    {
        for (int x = 0; x < width - 1; ++x)
        {
            int v1 = y * width + x + 1;
            int v2 = y * width + x + 2;
            int v3 = (y + 1) * width + x + 2;
            int v4 = (y + 1) * width + x + 1;

            fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v2, v2, v2, v3, v3, v3);
            fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v3, v3, v3, v4, v4, v4);
        }
    }

    fclose(objFile);
}

// void GeoTiffToObjConverter::writeObjFiles()
// {
//     int width = GDALGetRasterXSize(dataset);
//     int height = GDALGetRasterYSize(dataset);
//     int orthoWidth = GDALGetRasterXSize(orthoDataset);
//     int orthoHeight = GDALGetRasterYSize(orthoDataset);

//     int subdivisionSize = 200;
//     int subdivisionsX = width / subdivisionSize;
//     int subdivisionsY = height / subdivisionSize;

//     for (int i = 0; i < subdivisionsX; ++i)
//     {
//         for (int j = 0; j < subdivisionsY; ++j)
//         {
//             std::string outputObjFilePathStr(outputObjFilePath);
//             std::string baseName = outputObjFilePathStr.substr(0, outputObjFilePathStr.find_last_of("."));
//             std::string fileName = baseName + "_" + std::to_string(i) + "_" + std::to_string(j) + ".obj";
//             FILE *objFile = fopen(fileName.c_str(), "w");
//             if (!objFile)
//             {
//                 throw std::runtime_error("Unable to create the .obj file");
//             }

//             for (int y = j * subdivisionSize; y < (j + 1) * subdivisionSize; ++y)
//             {
//                 for (int x = i * subdivisionSize; x < (i + 1) * subdivisionSize; ++x)
//                 {
//                     double value;
//                     CPLErr err = GDALRasterIO(GDALGetRasterBand(dataset, 1), GF_Read, x, y, 1, 1, &value, 1, 1, GDT_Float64, 0, 0);
//                     if (err != CE_None)
//                     {
//                         throw std::runtime_error("Error reading raster data");
//                     }
//                     // ... same code as before ...

//                     fprintf(objFile, "v %d %f %d\n", x, 0.054 * value, y);
//                     float texCoordX = (float)(x - i * subdivisionSize) * (orthoWidth - 1) / (subdivisionSize - 1);
//                     float texCoordY = (float)(y - j * subdivisionSize) * (orthoHeight - 1) / (subdivisionSize - 1);
//                     fprintf(objFile, "vt %f %f\n", texCoordX / (orthoWidth - 1), texCoordY / (orthoHeight - 1));
//                     fprintf(objFile, "vn 0 1 0\n");
//                 }
//             }

//             // Write faces to the .obj file (simple grid)
//             for (int y = j * subdivisionSize; y < (j + 1) * subdivisionSize - 1; ++y)
//             {
//                 for (int x = i * subdivisionSize; x < (i + 1) * subdivisionSize - 1; ++x)
//                 {
//                     // ... same code as before ...
//                     int v1 = y * width + x + 1;
//                     int v2 = y * width + x + 2;
//                     int v3 = (y + 1) * width + x + 2;
//                     int v4 = (y + 1) * width + x + 1;

//                     fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v2, v2, v2, v3, v3, v3);
//                     fprintf(objFile, "f %d/%d/%d %d/%d/%d %d/%d/%d\n", v1, v1, v1, v3, v3, v3, v4, v4, v4);
//                 }
//             }

//             fclose(objFile);
//         }
//     }
// }
