#include "layer3d.h"
#include <iostream>

Layer3D::Layer3D(Object3DStorage *objectsStorage, int layerId, QString layerName):
	objectsStorage(objectsStorage), layerId(layerId), layerName(layerName), show_objects({})
{
	this->RequestUpdate();
}

Layer3D::~Layer3D()
{
	for (auto pair: this->objects)
		delete pair.second;

	delete layerWidget;
	delete layerItem;
}

void Layer3D::UpdateEmprise(const Emprise &emprise)
{
	std::map<int, Object3DFactory*> new_objects;
	std::vector<int> forget_objects;

	this->objectsStorage->GetObjectsInEmprise(
			emprise,
			new_objects,
			this->show_objects,
			forget_objects);



	for (const int id_obj: forget_objects) {
		if (this->objects.find(id_obj) != this->objects.end()) {
			//delete this->objects.at(id_obj);
			//this->objects.erase(id_obj);
		}
	}

	for (const auto &pair: new_objects) {
		if (this->objects.find(pair.first) != this->objects.end()) {
			delete this->objects[pair.first];
			std::cout << "Warning: overwrite object " << pair.first << std::endl;
		}
		this->objects[pair.first] = pair.second->New();
	}

}

void Layer3D::Draw(QOpenGLShaderProgram *shader)
{
	if (!this->visible) return;

	shader->setUniformValue("translation", this->objectsStorage->getTranslation());

	for (int id_obj: this->show_objects) {
		if (this->objects.find(id_obj) != this->objects.end())
			this->objects.at(id_obj)->Draw(shader);
		else
			std::cout << "Warning: unknowed object " << id_obj << std::endl;
	}
}
