#ifndef MNT3DSTORAGE_H
#define MNT3DSTORAGE_H


#include "i_openglcityview.h"

/**
 * @brief The MNT3dstorage class
 *
 * MNT3dstorage is a Object3DStorage
 */
class MNT3dstorage : public Object3DStorage
{
public:
    /**
     * @brief
     * make a MNT3dstorage from the base name of the .obj file
     * and the path to the texture file
     *
     * @param baseName:	path of .obj file
     * @param pathTexture:		path of texture file
     */
	MNT3dstorage(const std::string baseName, const std::string pathTexture);

    /**
     * @brief
     * make a MNT3dstorage from the base name of the .obj file,
     * the path to the texture file and the values of the translation in x and y
     *
     * @param baseName:	path of .obj file
     * @param xtranslate:	x translation value
     * @param ytranslate:	y translation value
     * @param pathTexture:		path of texture file
     */
	MNT3dstorage(const std::string baseName, const double xtranslate, const double ytranslate, const std::string pathTexture);


    /**
     * @brief MNT3dstorage destructor
     */
    ~MNT3dstorage();

	void GetObjectsInEmprise(
			const Emprise &/*emprise*/,
			std::map<int, Object3DFactory*> &new_buildings,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects);

	bool PickingObjectInfo(
			const QVector3D &/*p1*/,
			const QVector3D &/*p2*/,
			int &/*idObject*/,
			std::map<std::string, std::string> &/*data*/) {
		return false; // no picking
	}

private:
	std::map<int, Object3DFactory*> mnt_tiles;
	bool first;
};


#endif // MNT3DSTORAGE_H
