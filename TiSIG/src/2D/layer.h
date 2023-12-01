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
    int getZIndex() const;

    void setLayerVisible(bool m_visible);
    void setZIndex(int index);

    QListWidgetItem *layerItem;
    QWidget *layerWidget;

    QHBoxLayout *layout;
    QCheckBox *visibilityCheckbox;
    QLabel *layerLabel;

private:
    const QString name;
    bool visible;
    QGraphicsItemGroup *layerGroup;
    int zIndex;

};

#endif // LAYER_H
