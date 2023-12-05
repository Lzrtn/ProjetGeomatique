#ifndef EXEMPLEOBJECT3DSTORAGE_H
#define EXEMPLEOBJECT3DSTORAGE_H

#include "i_openglcityview.h"

/**
 * @brief The ExempleObject3DStorage class
 *
 * Exemple of Object3DStorage
 */
class ExempleObject3DStorage : public Object3DStorage
{
public:
	ExempleObject3DStorage();
	~ExempleObject3DStorage();

	void GetObjectsInEmprise(
			const Emprise &/*emprise*/,
			std::map<int, Object3DFactory*> &new_buildings,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects);

private:
	std::map<int, Object3DFactory*> buildings;
};

#endif // EXEMPLEOBJECT3DSTORAGE_H
