#ifndef RASTERIMPORT_H
#define RASTERIMPORT_H

#include "rasteritem.h"
#include "../outils/dbmanager.h"
#include <tuple>

/**
 * @brief Class for raster import from the database
 *
 * This class contains static methods that handle the creation of raster
 * items in the application, from fetched geotiff data stored in the database.
 */
class RasterImport
{
public:
    /**
     * @brief Creates a RasterItem with the given file path and the
     * associated extent retrieved from the database
     *
     * @param filePath Path of the image file
     * @return RasterItem item
     */
    static RasterItem* CreateRasterItemFromDb(const QString& filePath,DbManager& db);

private:
    /**
     * @brief Queries the geotiff table in the database to retrieve
     * extent data and formate it to be usable by RasterItem construction
     *
     * @param filePath Path of the image file
     * @return ID and extent of the most recent geotiff with the given file path
     */
    static std::tuple<int, QRectF> RetrieveRasterExtentFromDb(const QString& filePath, DbManager& db);

};

#endif // RASTERIMPORT_H
