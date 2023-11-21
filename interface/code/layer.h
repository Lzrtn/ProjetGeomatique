#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItemGroup>


class Layer
{
public:
    Layer(const QString& name, bool isVisible, QGraphicsItemGroup* layerGroup);
    QString getLayerName() const;
    bool isLayerVisible() const;
    QGraphicsItemGroup* getLayerGroup() const;
    void setLayerVisible(bool m_visible);

private:
    const QString name;
    bool visible;
    QGraphicsItemGroup* layerGroup;
};

#endif // LAYER_H
