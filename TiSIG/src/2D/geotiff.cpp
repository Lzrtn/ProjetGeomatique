#include "geotiff.h"

Geotiff::Geotiff(std::string fileName) : fileName(fileName)
{
    GDALAllRegister();
    Read();
}

Geotiff::~Geotiff()
{
    GDALClose(poDataset);
}

void Geotiff::Read()
{
    poDataset = (GDALDataset *)GDALOpen(fileName.c_str(), GA_ReadOnly);
    if (!poDataset)
    {
        std::cerr << "Error: Unable to open GeoTIFF file." << std::endl;
        return;
    }
    else
    {
        std::cout << "GeoTIFF file opened successfully." << std::endl;
    }
}

int Geotiff::CalculateNumberBands()
{
    if (poDataset != nullptr)
    {

        this->bandCount = poDataset->GetRasterCount();
    }
    else
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
    }
    return bandCount;
}

std::vector<int> Geotiff::CalculateImageDimensions()
{
    if (poDataset != nullptr)
    {
        this->width = GDALGetRasterXSize(poDataset);
        this->height = GDALGetRasterYSize(poDataset);
    }
    else
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
    }
    return {width, height};
}

std::vector<double> Geotiff::CalculateExtent()
{
    if (poDataset != nullptr)
    {
        double adfGeoTransform[6];
        if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
        {
            this->minX = adfGeoTransform[0];
            this->maxY = adfGeoTransform[3];
            this->maxX = minX + adfGeoTransform[1] * GDALGetRasterXSize(poDataset);
            this->minY = maxY + adfGeoTransform[5] * GDALGetRasterYSize(poDataset);
        }
        else
        {
            std::cerr << "Error: Unable to retrieve geotransform." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
    }

    return {minX, minY, maxX, maxY};
}

std::vector<double> Geotiff::CalculateResolution()
{
    if (poDataset != nullptr)
    {
        double adfGeoTransform[6];
        if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
        {
            // limit double to 5 decimal places

            this->pixelWidthResolution = adfGeoTransform[1];
            this->pixelHeightResolution = adfGeoTransform[5];
        }
        else
        {
            std::cerr << "Error: Unable to retrieve geotransform." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
    }
    pixelHeightResolution = std::round(pixelHeightResolution * 10000.0) / 10000.0;
    pixelWidthResolution = std::round(pixelWidthResolution * 10000.0) / 10000.0;

    return {pixelWidthResolution, pixelHeightResolution};
}

std::string Geotiff::GetFilePath()
{
    if (fileName.empty())
    {
        throw std::runtime_error("fileName is not initialized");
    }

    std::filesystem::path path(fileName);
    if (!std::filesystem::exists(path))
    {
        throw std::runtime_error("Path does not exist");
    }

    path = std::filesystem::canonical(path);
    std::string pathStr = path.string();
    filePath = pathStr;
    return filePath;
}

std::vector<double> Geotiff::CalculateOrigin()
{
    CalculateExtent();
    return {minX, maxY};
}

void Geotiff::WriteGeotiffAndMetadataToPostgis(DbManager &db)
{
    if (poDataset == nullptr)
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
        return;
    }

    CalculateImageDimensions();

    CalculateNumberBands();
    CalculateExtent();
    GetFilePath();

    pqxx::binarystring rasterBinary("");

    for (int bandIndex = 1; bandIndex <= bandCount; ++bandIndex)
    {
        GDALRasterBand *band = poDataset->GetRasterBand(bandIndex);
        std::vector<uint16_t> rasterData(width * height);
        CPLErr err = band->RasterIO(GF_Read, 0, 0, width, height, rasterData.data(), width, height, GDT_UInt16, 0, 0);
        if (err != CE_None)
        {
            // Handle the error
            std::cerr << "Error reading raster data: " << CPLGetLastErrorMsg() << std::endl;
            return;
        }
        pqxx::binarystring newBinary(reinterpret_cast<const char *>(rasterData.data()), rasterData.size() * sizeof(uint16_t));
        std::string rasterStr(rasterBinary.get(), rasterBinary.size());
        std::string newStr(newBinary.get(), newBinary.size());
        rasterStr += newStr;
        rasterBinary = pqxx::binarystring(rasterStr.c_str(), rasterStr.size());
    }
    // Writing to the database
    db.CreateTable("CREATE TABLE IF NOT EXISTS geotiff_data (raster_data BYTEA ,height INT, width INT, band_count INT, min_x DOUBLE PRECISION, max_x DOUBLE PRECISION, min_y DOUBLE PRECISION, max_y DOUBLE PRECISION, pixel_height_resolution DOUBLE PRECISION, pixel_width_resolution DOUBLE PRECISION, file_path TEXT)");
    std::string query = "INSERT INTO geotiff_data (raster_data, height, width, band_count, min_x, max_x, min_y, max_y, pixel_height_resolution, pixel_width_resolution, file_path) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)";
    std::vector<std::variant<int, double, std::string, pqxx::binarystring>> params = {rasterBinary, height, width, bandCount, minX, maxX, minY, maxY, pixelHeightResolution, pixelWidthResolution, filePath};
    // Construct the SQL query string with parameters
    for (size_t i = 0; i < params.size(); ++i)
    {
        std::string param;
        if (std::holds_alternative<int>(params[i]))
        {
            param = std::to_string(std::get<int>(params[i]));
        }
        else if (std::holds_alternative<double>(params[i]))
        {
            param = std::to_string(std::get<double>(params[i]));
            // Replace the decimal comma with a decimal point
            std::replace(param.begin(), param.end(), ',', '.');
        }
        else if (std::holds_alternative<std::string>(params[i]))
        {
            std::string str_param = std::get<std::string>(params[i]);

            size_t pos = str_param.find("'");
            while (pos != std::string::npos)
            {
                str_param.replace(pos, 1, "''");
                pos = str_param.find("'", pos + 2);
            }

            param = "'" + str_param + "'";
        }
        else if (std::holds_alternative<pqxx::binarystring>(params[i]))
        {
            std::string binary_data = std::get<pqxx::binarystring>(params[i]).str();
            std::stringstream ss;
            ss << "E'\\\\x";
            for (unsigned char c : binary_data)
            {
                ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
            }
            ss << "'";
            param = ss.str();
        }
        std::string placeholder = "$" + std::to_string(i + 1);
        size_t pos = query.find(placeholder);
        if (pos != std::string::npos)
        {
            query.replace(pos, placeholder.length(), param);
        }
    }

    db.Request(query);
}

std::vector<double> Geotiff::PixelToGeoCoordinates(int pixelX, int pixelY)
{
    if (poDataset != nullptr)
    {
        double adfGeoTransform[6];
        if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
        {
            double geoX = adfGeoTransform[0] + pixelX * adfGeoTransform[1] + pixelY * adfGeoTransform[2];
            double geoY = adfGeoTransform[3] + pixelX * adfGeoTransform[4] + pixelY * adfGeoTransform[5];

            return {geoX, geoY};
        }
        else
        {
            std::cerr << "Error: Unable to retrieve geotransform." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
    }

    // Return an empty vector in case of an error
    return {};
}

int Geotiff::GetNumberOfImagesStored(DbManager &db)
{
    db.Request("SELECT Count(*) FROM geotiff_data");
    std::string result = db.ParseResult();

    std::size_t pos = result.find('='); // Find the position of '=' in the string

    if (pos != std::string::npos)
    {
        result = result.substr(pos + 1); // Get the substring after '='
    }
    return std::stoi(result);
}

std::vector<float> Geotiff::GetPixelValue(int pixelX, int pixelY)
{
    std::vector<float> pixelValue;
    for (int bandIndex = 1; bandIndex <= bandCount; ++bandIndex)
    {
        GDALRasterBand *band = poDataset->GetRasterBand(bandIndex);
        float rasterData;
        CPLErr err = band->RasterIO(GF_Read, pixelX, pixelY, 1, 1, &rasterData, 1, 1, GDT_Float32, 0, 0);
        if (err != CE_None)
        {
            // Handle the error
            std::cerr << "Error reading raster data: " << CPLGetLastErrorMsg() << std::endl;
            return {};
        }
        pixelValue.push_back(rasterData);
    }
    return pixelValue;
}

std::vector<int> Geotiff::GeoCoordinatesToPixel(double geoX, double geoY)
{
    if (poDataset != nullptr)
    {
        double adfGeoTransform[6];
        if (poDataset->GetGeoTransform(adfGeoTransform) == CE_None)
        {
            double pixelX = (geoX - adfGeoTransform[0]) / adfGeoTransform[1];
            double pixelY = (geoY - adfGeoTransform[3]) / adfGeoTransform[5];

            return {static_cast<int>(pixelX), static_cast<int>(pixelY)};
        }
        else
        {
            std::cerr << "Error: Unable to retrieve geotransform." << std::endl;
        }
    }
    else
    {
        std::cerr << "Error: GeoTIFF not opened." << std::endl;
    }

    // Return an empty vector in case of an error
    return {};
}
