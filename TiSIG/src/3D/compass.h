#ifndef COMPASS_H
#define COMPASS_H

#include "object3d.h"

/**
 * @brief The Compass class make a compass object (Object3D) to inform about the camera orientation
 */
class Compass: public Object3D
{
public:
	/**
	 * @brief Compass
	 *
	 * Similar than Object3D constructor
	 *
	 * @see Object3D::Object3D
	 */
	Compass(const std::vector<QVector3D> & position, const std::vector<QVector3D> & normal,
			const std::vector<QVector2D> & textCoord, const std::string & textPath) :
		Object3D(position, normal, textCoord, textPath)
	{};

};

/**
 * @brief The CompassFactory class genere a compass object. Is similar than Building3DFactory
 *
 * @see BuildingFactory
 */
class CompassFactory : public Object3DFactory
{
public:
	CompassFactory();

	Compass * New() const;

private:
	std::vector<QVector3D> position;
	std::vector<QVector3D> normal;
	std::vector<QVector2D> textCoord;
	std::string textPath;
};

#endif // COMPASS_H
