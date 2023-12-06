#ifndef LAYER_H
#define LAYER_H

#include <QGraphicsItemGroup>
#include <QWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>


/**
 * @brief The Layer class represents a layer in a graphics scene.
 * 
 * A layer is a container for graphical items that can be displayed or hidden.
 * It has a name, visibility status, and a Z-index for ordering layers.
 */
class Layer
{
public:
    /**
     * @brief Constructs a Layer object with the given name, visibility status, and layer group.
     * 
     * @param name The name of the layer.
     * @param isVisible The visibility status of the layer.
     * @param layerGroup The QGraphicsItemGroup associated with the layer.
     */
    Layer(const QString& name, bool isVisible, QGraphicsItemGroup* layerGroup);

    /**
     * @brief Destructor for the Layer class.
     */
    ~Layer();

    /**
     * @brief Checks if the layer is visible.
     * 
     * @return true if the layer is visible, false otherwise.
     */
    bool isLayerVisible() const;

    /**
     * @brief Gets the QGraphicsItemGroup associated with the layer.
     * 
     * @return A pointer to the QGraphicsItemGroup.
     */
    QGraphicsItemGroup* getLayerGroup() const;

    /**
     * @brief Gets the name of the layer.
     * 
     * @return The name of the layer.
     */
    QString getLayerName() const;

    /**
     * @brief Gets the Z-index of the layer.
     * 
     * @return The Z-index of the layer.
     */
    int getZIndex() const;

    /**
     * @brief Sets the visibility status of the layer.
     * 
     * @param m_visible The visibility status to set.
     */
    void setLayerVisible(bool m_visible);

    /**
     * @brief Sets the Z-index of the layer.
     * 
     * @param index The Z-index to set.
     */
    void setZIndex(int index);

    QListWidgetItem *layerItem; /**< The QListWidgetItem associated with the layer. */
    QWidget *layerWidget; /**< The QWidget associated with the layer. */
    QHBoxLayout *layout; /**< The QHBoxLayout associated with the layer. */
    QCheckBox *visibilityCheckbox; /**< The QCheckBox associated with the layer's visibility. */
    QLabel *layerLabel; /**< The QLabel associated with the layer's name. */

private:
    const QString name; /**< The name of the layer. */
    bool visible; /**< The visibility status of the layer. */
    QGraphicsItemGroup *layerGroup; /**< The QGraphicsItemGroup associated with the layer. */
    int zIndex; /**< The Z-index of the layer. */
};


#endif // LAYER_H
