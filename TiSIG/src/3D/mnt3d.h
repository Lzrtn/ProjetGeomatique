#ifndef MNT3D_H
#define MNT3D_H

#include "object3d.h"

/**
 * @brief The MNT3D class
 *
 * Contain 3D object setted by .obj & .png file (gerenated by .mnt)
 *
 * @see Object3D
 * @see MNT3DFactory
 */
class MNT3D : public Object3D
{
public:
	/**
	 * @brief
	 * make a mnt from obj
	 *
	 * @param pathTexture:	path of texture image
	 * @param pathObj:		path of .obj file
	 *
	 * @see Object3D::Object3D
	 */
	MNT3D(const std::string pathObj, const std::string & pathTexture) :
		Object3D(pathObj, pathTexture)
	{};
};

/**
 * @brief The MNT3DFactory class
 * generator for MNT3D
 *
 * @see ObjectFactory
 * @see MNT3D
 */
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
