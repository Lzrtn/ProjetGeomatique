#include "layer.h"
#include <iostream>

Layer::Layer(const QString& name, bool isVisible, QGraphicsItemGroup* layerGroup)
    : name(name), visible(isVisible), layerGroup(layerGroup)
{

}

QString Layer::getLayerName() const {
    return name;
}

bool Layer::isLayerVisible() const {
    return visible;
}

void Layer::setLayerVisible(bool m_visible) {
    visible = m_visible;
}


QGraphicsItemGroup* Layer::getLayerGroup() const {
    return layerGroup;
}
