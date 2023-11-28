#ifndef RASTERIMPORT_H
#define RASTERIMPORT_H

#include "rasteritem.h"
#include "../outils/dbmanager.h"

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
     * @param fileName Path of the image file
     * @return RasterItem item
     */
    static RasterItem* CreateRasterItemFromDb(const QString& fileName,DbManager& db);

private:
    /**
     * @brief Queries the geotiff table in the database to retrieve
     * extent data and formate it to be usable by RasterItem construction
     *
     * @param fileName Path of the image file
     * @return Extent of the raster
     */
    static QRectF RetrieveRasterExtentFromDb(const QString& fileName, DbManager& db);

};

#endif // RASTERIMPORT_H
