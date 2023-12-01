#ifndef BUILDING3D_H
#define BUILDING3D_H

#include "object3d.h"

/**
 * @brief The Building3D class
 * Object that contains building geometry data (vertex, texture, ...)
 */
class Building3D : public Object3D
{
public:

	/**
	 * @brief
	 * make a building with geometrics properties
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param position  vertices 3D of points of faces
	 * @param normal    vertices 3D of normals
	 * @param textCord  vertices 2D of points in texture
	 * @param textPath  path of texture image
	 */
	Building3D(const std::vector<QVector3D> & position, const std::vector<QVector3D> & normal,
			const std::vector<QVector2D> & textCoord, const std::string & textPath) :
		Object3D(position, normal, textCoord, textPath)
	{};

	/**
	 * @brief
	 * make a building with geometrics properties
	 *
	 * Compute automaticly normals
	 * @warning need direct-oriented faces to compute normals
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param position  vertices 3D of points of faces
	 * @param textCord  vertices 2D of points in texture
	 * @param textPath  path of texture image
	 */
	Building3D(const std::vector<QVector3D> & position,
			const std::vector<QVector2D> & textCoord, const std::string & textPath) :
		Object3D(position, textCoord, textPath)
	{};

	Building3D(const std::string pathObj,
			const std::string & textPath) :
		Object3D(pathObj, textPath)
	{};

private:
};

class Building3DFactory : public Object3DFactory
{
public:
	/**
	 * @brief Building3DFactory produce buildings from this geometry
	 *
	 * All faces may have 3 points;
	 *
	 * @param position: 3D coordinate of points
	 * @param normal: 3D vector normal to faces. Currently unused, but may
	 *   be used in a futur version with light effects
	 * @param textCoord: 2D coordinate of point in texture image
	 * @param textPath: path of texture image. Only one image can be used
	 */
	Building3DFactory(
			const std::vector<QVector3D> &position,
			const std::vector<QVector3D> &normal,
			const std::vector<QVector2D> &textCoord,
			const std::string &textPath);

	/**
	 * @brief Building3DFactory produce buildings for test
	 *
	 * @param version: version of building (between 0 and 1 included)
	 */
	Building3DFactory(const int version = 0);

	Building3DFactory(const QVector3D pt);

	/**
	 * @brief Genere a building
	 *
	 * @Warning: risk of memory lost. equivalent to `new Building(...)`
	 * Remember to delete building after use
	 */
	Building3D * New() const;

private:
	std::vector<QVector3D> position;
	std::vector<QVector3D> normal;
	std::vector<QVector2D> textCoord;
	std::string textPath;
	std::string objPath;
};

#endif // BUILDING3D_H
