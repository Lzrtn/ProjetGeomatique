#ifndef I_OPENGLCITYVIEW_H
#define I_OPENGLCITYVIEW_H

#include "building3d.h"
#include <map>

class Emprise {
public:
	// 3D coordinates for volume
	QVector3D a, b, c, d, e, f, g, h;

	// 3D coordinates for ground plan (z=0)
	QVector3D g_a, g_b, g_c, g_d;

	void updateGroundCoordinates();
};

class OpenGLCityView_BuildingStorage {
	virtual std::map<int, Building3DFactory> getBuildingsInEmprise(const Emprise &emprise) = 0;
};

#endif // I_OPENGLCITYVIEW_H
