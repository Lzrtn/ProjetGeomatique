#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItemGroup>
#include <QWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>


class Layer
{
public:
    Layer(const QString& name, bool isVisible, QGraphicsItemGroup* layerGroup);
    ~Layer();

    bool isLayerVisible() const;
    QGraphicsItemGroup* getLayerGroup() const;
    QString getLayerName() const;

    void setLayerVisible(bool m_visible);

    QListWidgetItem *layerItem;
    QWidget *layerWidget;


private:
    const QString name;
    bool visible;

    QGraphicsItemGroup *layerGroup;

};

#endif // LAYER_H
