#ifndef RASTERITEM_H
#define RASTERITEM_H

#include <QGraphicsPixmapItem>

/**
 * @brief Class representing a raster item, derived from the QGraphicsPixmapItem class
 *
 * This class is an extension of the QGraphicsPixmapItem class, suited to handle
 * the extent of a raster as well as creating an item directly from the image file.
 */
class RasterItem : public QGraphicsPixmapItem
{


public:

    /**
     * @brief RasterItem class constructor from a pixmap
     *
     * @param pixmap The pixmap to set for the item
     * @param parent Optional parent item
     */
    RasterItem(const QPixmap& pixmap, QGraphicsItem* parent = nullptr);


    /**
     * @brief RasterItem class constructor from the image file name
     *
     * @param fileName Path of the image file
     * @param parent Optional parent item
     */
    RasterItem(const QString& fileName, QGraphicsItem* parent = nullptr);


    /**
     * @brief RasterItem class constructor from the image file name, that
     * takes an extent as an argument and directly assign it to the raster item
     *
     * @param fileName Path of the image file
     * @param extent Extent that the raster must have
     * @param parent Optional parent item
     */
    RasterItem(const QString& fileName, const QRectF & extent, QGraphicsItem* parent = nullptr);


    /**
     * @brief Set the position of the RasterItem and rescale it to fit
     * the given extent in the scene coordinate system
     *
     * @param extent Extent that the raster must have
     */
    void FitToExtent(const QRectF & extent);

    /**
     * @brief Get the extent of the raster in the scene coordinate system
     *
     * @return The extent of the raster
     */
    QRectF getExtent();

};

#endif // RASTERITEM_H
