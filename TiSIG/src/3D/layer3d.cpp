#include "layer3d.h"

Layer3D::Layer3D(Object3DStorage *objectsStorage, int layerId, QString layerName):
    objectsStorage(objectsStorage), layerId(layerId), layerName(layerName), show_objects({})
{

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

	for (const auto &pair: new_objects) {
		if (this->objects.find(pair.first) != this->objects.end())
			delete this->objects[pair.first];
		this->objects[pair.first] = pair.second->New();
	}

	for (const int id_obj: forget_objects) {
		if (this->objects.find(id_obj) != this->objects.end()) {
			delete this->objects[id_obj];
			this->objects.erase(id_obj);
		}
	}

}

void Layer3D::Draw(QOpenGLShaderProgram *shader)
{
	if (!this->visible) return;

	shader->setUniformValue("translation", this->objectsStorage->getTranslation());

	for (int id_obj: this->show_objects) {
		this->objects[id_obj]->Draw(shader);
	}
}
