#ifndef EXEMPLEOBJECT3DSTORAGE_H
#define EXEMPLEOBJECT3DSTORAGE_H

#include "i_openglcityview.h"

/**
 * @brief The ExempleObject3DStorage class
 *
 * Exemple of Object3DStorage
 *
 * @see Object3DStorage
 */
class ExempleObject3DStorage : public Object3DStorage
{
public:
	/**
	 * @brief ExempleObject3DStorage
	 * Make a layer as example and tests, with buildings and mnt
	 * @see Building3D
	 * @see MNT3D
	 * @see Object3DStorage
	 */
	ExempleObject3DStorage();
	/**
	 * @brief destructor
	 * @see ExempleObject3DStorage::ExempleObject3DStorage (constructor)
	 */
	~ExempleObject3DStorage();

	/**
	 * @brief GetObjectsInEmprise
	 *
	 * @see Object3DStorage::GetObjectsInEmprise
	 */
	void GetObjectsInEmprise(
			const Emprise &/*emprise*/,
			std::map<int, Object3DFactory*> &new_buildings,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects);

	/**
	 * @brief PickingObjectInfo
	 *
	 * @see Object3DStorage::PickingObjectInfo
	 */
	bool PickingObjectInfo(
			const QVector3D &p1,
			const QVector3D &p2,
			int &idObject,
			std::map<std::string, std::string> &data);

private:
	std::map<int, Object3DFactory*> buildings;
	bool firstUpdate = true;
};

#endif // EXEMPLEOBJECT3DSTORAGE_H
