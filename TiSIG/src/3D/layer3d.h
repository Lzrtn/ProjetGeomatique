#ifndef LAYER3D_H
#define LAYER3D_H

#include "object3d.h"
#include "i_openglcityview.h"

/**
 * @brief The Layer3D class
 *
 * Group of objects 3D linked to Object3DStorage to be displayed into OpenGLCityView
 */
class Layer3D
{
private:
	Object3DStorage * objectsStorage;
	bool visible = true;

	std::map<int, Object3D*> objects;

	std::vector<int> show_objects;

public:
	bool isVisible() const { return this->visible; }
	void setVisible(const bool visible) { this->visible = visible; }

	Layer3D(Object3DStorage * objectsStorage);
	~Layer3D();

	void UpdateEmprise(const Emprise & emprise);

	bool PickingObjectInfo(
			const QVector3D &p1,
			const QVector3D &p2,
			int &idObject,
			std::map<std::string, std::string> &data) {
		return this->objectsStorage->PickingObjectInfo(p1, p2, idObject, data);
	}

	void Draw(QOpenGLShaderProgram *shader);
};

#endif // LAYER3D_H
