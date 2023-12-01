#include "rasteritem.h"
#include <QGraphicsPixmapItem>

RasterItem::RasterItem(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
{

}

RasterItem::RasterItem(const QString& filePath, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{

    QPixmap pixmap(filePath);
    setPixmap(pixmap);
}

RasterItem::RasterItem(const QString& filePath, const QRectF & extent, const int id, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent), id(id)
{

    QPixmap pixmap(filePath);
    setPixmap(pixmap);

    FitToExtent(extent);
}

void RasterItem::FitToExtent(const QRectF & extent)
{

    setPos(mapFromScene(extent.topLeft()));

    qreal scaleX = extent.width() / pixmap().width();
    qreal scaleY = extent.height() / pixmap().height();

    setTransform(QTransform().scale(scaleX, scaleY));

}

QRectF RasterItem::getExtent()
{
    return mapRectToScene(boundingRect());
}

int RasterItem::getId()
{
    return id;
}

