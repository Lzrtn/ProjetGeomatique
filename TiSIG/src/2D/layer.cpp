#include "layer.h"
#include <iostream>

Layer::Layer(const QString& name, bool isVisible, QGraphicsItemGroup* layerGroup)
    : name(name), visible(isVisible), layerGroup(layerGroup)
{

}


Layer::~Layer() {

    delete layerWidget;
    delete layerItem;
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

int Layer::getZIndex() const{
    return zIndex;
}

void Layer::setZIndex(int index){
    zIndex = index;
}


QGraphicsItemGroup* Layer::getLayerGroup() const {
    return layerGroup;
}

