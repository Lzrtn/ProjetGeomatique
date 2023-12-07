#ifndef MNT3D_H
#define MNT3D_H

#include "object3d.h"

class MNT3D : public Object3D
{
public:
	/**
	 * @brief
	 * make a mnt from obj
	 *
	 * @param pathTexture:	path of texture image
	 * @param pathObj:		path of .obj file
	 */
	MNT3D(const std::string pathObj, const std::string & pathTexture) :
		Object3D(pathObj, pathTexture)
	{};
};

class MNT3DFactory : public Object3DFactory
{
public:
	/**
	 * @brief MNT3DFactory produce MNT from this geometry
	 *
	 * All faces may have 3 points;
	 *
	 * @param textPath: path of texture image. Only one image can be used
	 */
	MNT3DFactory(const std::string &pathObj, const std::string &pathTexture) {
		this->pathObj = pathObj;
		this->pathTexture = pathTexture;
	}

	MNT3DFactory(int version) {
		if (version == 0) {
			this->pathObj = "../src/data/Tests/cube.obj";
			this->pathTexture = ":/cube.png";
		} else {
			//this->pathObj = "../src/data/DONNEES_BDORTHO/MNT.obj";
			this->pathObj = "../src/data/DONNEES_BDORTHO/decoupe_25m.obj";
			this->pathTexture = "../src/data/DONNEES_BDORTHO/Lyon5e_2m.png";
		}
	}

	/**
	 * @brief Genere a mnt
	 *
	 * @warning: risk of memory lost. equivalent to `new ...(...)`
	 * Remember to delete building after use
	 */
	MNT3D *New() const;

private:
	std::string pathTexture;
	std::string pathObj;
};
#endif // MNT3D_H
