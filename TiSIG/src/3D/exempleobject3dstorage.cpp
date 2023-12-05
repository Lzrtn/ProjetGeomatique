#include "exempleobject3dstorage.h"
#include "building3d.h"
#include "mnt3d.h"

ExempleObject3DStorage::ExempleObject3DStorage()
{
	this->buildings = {
		{0, new Building3DFactory(0)},
		{1, new Building3DFactory(1)},
		{2, new Building3DFactory(2)},
		{3, new Building3DFactory(3)},
		{4, new MNT3DFactory(0)},
	};

	this->setTranslation({5, 0, 0});
}

ExempleObject3DStorage::~ExempleObject3DStorage()
{
	for (auto pair: this->buildings) {
		delete pair.second;
	}
}

void ExempleObject3DStorage::GetObjectsInEmprise(
		const Emprise &/*emprise*/,
		std::map<int, Object3DFactory *> &new_buildings,
		std::vector<int> &show_objects,
		std::vector<int> &forget_objects)
{
	new_buildings.clear();

	// compute displayed buildings with emprise
	show_objects = {0, 1, 2, 3, 4};

	// give new buildings
	for (int id: show_objects) {
		new_buildings[id] = buildings[id];
	}

	// building id to forget
	forget_objects={};
}
