#ifndef LAYER3D_H
#define LAYER3D_H

#include "object3d.h"
#include "i_openglcityview.h"

#include <QWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QLabel>

/**
 * @brief The Layer3D class
 *
 * Group of objects 3D linked to Object3DStorage to be displayed into OpenGLCityView
 */
/**
 * @brief The Layer3D class represents a 3D layer in the application.
 *
 * This class inherits from the Updatable class and provides functionality for managing 3D objects in the layer.
 */
class Layer3D : public Updatable
{
private:
	Object3DStorage * objectsStorage; 	/**< Pointer to the storage of 3D objects. */
	bool visible = true; 				/**< Flag indicating whether the layer is visible or not. */
	int layerId; 						/**< The ID of the layer. */
	QString layerName; 					/**< The name of the layer. */

	std::map<int, Object3D*> objects; 	/**< Map of 3D objects in the layer. */

	std::vector<int> show_objects; 		/**< Vector of IDs of objects to be shown in the layer. */

public:
	/**
	 * @brief Check if the layer is visible.
	 * @return True if the layer is visible, false otherwise.
	 */
	bool isVisible() const { return this->visible; }

	/**
	 * @brief Set the visibility of the layer.
	 * @param visible The visibility flag to set.
	 */
	void setVisible(const bool visible) {
		this->visible = visible;
		this->RequestUpdate();
	}

	/**
	 * @brief Construct a new Layer3D object.
	 * @param objectsStorage Pointer to the storage of 3D objects.
	 * @param layerId The ID of the layer.
	 * @param layerName The name of the layer.
	 */
	Layer3D(Object3DStorage * objectsStorage, int layerId, QString layerName);

	/**
	 * @brief Destroy the Layer3D object.
	 */
	~Layer3D();

	/**
	 * @brief Update the extent of the layer.
	 * @param emprise The new extent of the layer.
	 */
	void UpdateEmprise(const Emprise & emprise);

	bool PickingObjectInfo(
			const QVector3D &p1,
			const QVector3D &p2,
			int &idObject,
			std::map<std::string, std::string> &data) {
		return this->objectsStorage->PickingObjectInfo(p1, p2, idObject, data);
	}

	QVector3D getCameraInitPosition() const { return this->objectsStorage->getCameraInitPosition() + this->objectsStorage->getTranslation(); }

	/**
	 * @brief Draw the layer using the specified shader program.
	 * @param shader The shader program to use for drawing.
	 */
	void Draw(QOpenGLShaderProgram *shader);

	/**
	 * @brief Get the ID of the layer.
	 * @return The ID of the layer.
	 */
	int getLayerId() const { return layerId; }

	/**
	 * @brief Get the name of the layer.
	 * @return The name of the layer.
	 */
	QString getLayerName() const { return layerName; }

	QListWidgetItem *layerItem; 	/**< Pointer to the list widget item representing the layer. */
	QWidget *layerWidget; 			/**< Pointer to the widget representing the layer. 			 */
	QHBoxLayout *layout; 			/**< Pointer to the layout of the layer widget. 			 */
	QCheckBox *visibilityCheckbox; 	/**< Pointer to the checkbox for toggling layer visibility.  */
	QLabel *layerLabel; 			/**< Pointer to the label displaying the layer name. 		 */
};

#endif // LAYER3D_H
