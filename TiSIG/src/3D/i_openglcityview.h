#ifndef I_OPENGLCITYVIEW_H
#define I_OPENGLCITYVIEW_H

#include "building3d.h"
#include "object3d.h"
#include <map>

class ICameraDisplayInfo {
public:
	virtual void Display3DCameraCoordinates(QVector3D camPosition) = 0;
	virtual void Display3DZoomLevel(float zoom) = 0;
};
class IPicking3DDisplayInfo {
public:
	virtual void Display3DPickingResult(const std::map<std::string, std::string> &data) = 0;
};


/**
 * @brief The Emprise class
 * Give the current emprise of a view
 *
 * emprise is defined with 8 points: a, b, c, d, e, f, g, h;
 *
 * Prologation of point to ground: g_a, g_b, g_c, g_d;
 */
class Emprise {
public:
	// 3D coordinates for volume
	QVector3D a, b, c, d, e, f, g, h;

	// 3D coordinates for ground plan (z=0)
	QVector3D g_a, g_b, g_c, g_d;

	void updateGroundCoordinates() {
		// intersection between line (a,e) [resp. (b,f); (c,g); (e,h)] and plane (z=0)
		// if no intersect, first point will be chosen
		g_a = e.z() != a.z() ? a - a.z() * (e-a) / (e.z() - a.z()) : a;
		g_b = f.z() != b.z() ? b - b.z() * (f-b) / (f.z() - b.z()) : b;
		g_c = g.z() != c.z() ? c - c.z() * (g-c) / (g.z() - c.z()) : c;
		g_d = h.z() != d.z() ? d - d.z() * (h-d) / (h.z() - d.z()) : d;
	}

	bool operator!= (const Emprise & o) const {
		return !(a==o.a && b==o.b && c==o.c && d==o.d
			  && e==o.e && f==o.f && g==o.g && h==o.h);
	}

};

/**
 * @brief The OpenGLCityView_BuildingStorage class
 * OBSELETE
 *
 * @see Object3DStorage
 */
class OpenGLCityView_BuildingStorage {
public:
	virtual std::map<int, Building3DFactory> getBuildingsInEmprise(const Emprise &emprise) = 0;

	virtual void GetObjectsInEmprise(
			const Emprise &emprise,
			std::map<int, Object3DFactory> &new_objects,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects) = 0;
};

/**
 * @brief The Object3DStorage class
 * Strorage systeme that give object3D to layer
 *
 * @see Object3D, Object3DFactory
 * @see Layer3D
 * @see OpenGLcityView
 */
class Object3DStorage {
public:
	/**
	 * @brief Default destructor
	 */
	virtual ~Object3DStorage() = default;

	/**
	 * @brief GetObjectsInEmprise
	 * @param emprise :			(input)  current emprise of view
	 * @param new_objects :		(output) map of new objects
	 * @param show_objects :	(output) id of visibles objects
	 * @param forget_objects :	(output) id of objects to remove of memory
	 */
	virtual void GetObjectsInEmprise(
			const Emprise &emprise,
			std::map<int, Object3DFactory*> &new_objects,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects) = 0;

	virtual bool PickingObjectInfo(
			const QVector3D &p1,
			const QVector3D &p2,
			int &idObject,
			std::map<std::string, std::string> &data) = 0;

	/**
	 * @brief getTranslation
	 *
	 * Coordinates are defined with a translation. Get this translation.
	 * @return translation
	 */
	QVector3D getTranslation() const { return this->translation; };

protected:
	/**
	 * @brief getTranslation
	 *
	 * Coordinates are defined with a translation. Set this translation.
	 */
	void setTranslation(const QVector3D & translation) { this->translation = translation; }

private:
	QVector3D translation = {0,0,0};
};

#endif // I_OPENGLCITYVIEW_H
