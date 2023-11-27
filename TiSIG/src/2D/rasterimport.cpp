#include "rasterimport.h"
#include <string>


RasterItem* RasterImport::CreateRasterItemFromDb(const QString& fileName,DbManager& db)
{

    QRectF extent = RetrieveRasterExtentFromDb(fileName,db);
    RasterItem* rasterItem = new RasterItem(fileName, extent);

    return rasterItem;

}

QRectF RasterImport::RetrieveRasterExtentFromDb(const QString& fileName,DbManager& db)
{


    std::string request = "SELECT min_x,min_y,max_x,max_y FROM geotiff_data WHERE file_path ='" + fileName.toStdString()+"'";
    db.Request(request);
    std::vector<std::vector<std::string>> parsedData = db.ArrayParseResult();


    double xMin = std::stod(parsedData[0][0]);
    double yMax = -std::stod(parsedData[0][1]);
    double xMax = std::stod(parsedData[0][2]);
    double yMin = -std::stod(parsedData[0][3]);

    QRectF extent(QPointF(xMin,yMin),QPointF(xMax,yMax));


    return extent;
}
