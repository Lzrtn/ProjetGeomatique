#include "rasteritem.h"
#include <QGraphicsPixmapItem>

RasterItem::RasterItem(const QPixmap& pixmap, QGraphicsItem* parent)
    : QGraphicsPixmapItem(pixmap, parent)
{

}

RasterItem::RasterItem(const QString& fileName, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{

    QPixmap pixmap(fileName);
    setPixmap(pixmap);
}

RasterItem::RasterItem(const QString& fileName, const QRectF & extent, QGraphicsItem* parent)
    : QGraphicsPixmapItem(parent)
{

    QPixmap pixmap(fileName);
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

