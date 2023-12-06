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
class Layer3D : public Updatable
{
private:
	Object3DStorage * objectsStorage;
	bool visible = true;
    int layerId;
    QString layerName;

	std::map<int, Object3D*> objects;

	std::vector<int> show_objects;

public:
	bool isVisible() const { return this->visible; }
	void setVisible(const bool visible) {
		this->visible = visible;
		this->RequestUpdate();
	}

    Layer3D(Object3DStorage * objectsStorage, int layerId, QString layerName);
	~Layer3D();

	void UpdateEmprise(const Emprise & emprise);

	void Draw(QOpenGLShaderProgram *shader);

    int getLayerId();
    QString getLayerName();

    QListWidgetItem *layerItem;
    QWidget *layerWidget;

    QHBoxLayout *layout;
    QCheckBox *visibilityCheckbox;
    QLabel *layerLabel;
};

#endif // LAYER3D_H
