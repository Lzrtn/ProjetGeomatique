#include "rasterimport.h"
#include <string>


RasterItem* RasterImport::CreateRasterItemFromDb(const QString& filePath,DbManager& db)
{
    auto resultTuple =RetrieveRasterExtentFromDb(filePath,db);

    int rasterId = std::get<0>(resultTuple);
    QRectF extent = std::get<1>(resultTuple);

    RasterItem* rasterItem = new RasterItem(filePath,extent,rasterId);

    return rasterItem;

}

std::tuple<int, QRectF> RasterImport::RetrieveRasterExtentFromDb(const QString& filePath,DbManager& db)
{


    std::string request = "SELECT min_x,min_y,max_x,max_y,id FROM geotiff_data WHERE file_path ='" + filePath.toStdString()
            +"' ORDER BY id DESC LIMIT 1";

    db.Request(request);
    std::vector<std::vector<std::string>> parsedData = db.ArrayParseResult();

    int rasterId = std::stoi(parsedData[0][4]);

    double xMin = std::stod(parsedData[0][0]);
    double yMax = -std::stod(parsedData[0][1]);
    double xMax = std::stod(parsedData[0][2]);
    double yMin = -std::stod(parsedData[0][3]);

    QRectF extent(QPointF(xMin,yMin),QPointF(xMax,yMax));


    return std::make_tuple(rasterId, extent);
}
