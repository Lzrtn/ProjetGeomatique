#ifndef I_OPENGLCITYVIEW_H
#define I_OPENGLCITYVIEW_H

#include "building3d.h"
#include "object3d.h"
#include <map>

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

class OpenGLCityView_BuildingStorage {
public:
	virtual std::map<int, Building3DFactory> getBuildingsInEmprise(const Emprise &emprise) = 0;

	virtual void GetObjectsInEmprise(
			const Emprise &emprise,
			std::map<int, Object3DFactory> &new_objects,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects) = 0;
};

class OpenGLCityView_Object3DStorageLayer {
public:
	virtual void GetObjectsInEmprise(
			const Emprise &emprise,
			std::map<int, Object3DFactory> &new_objects,
			std::vector<int> &show_objects,
			std::vector<int> &forget_objects) = 0;

	QVector3D getTranslation() const { return this->translation; };
	void setTranslation(const QVector3D & translation) { this->translation = translation; }

	bool isVisible() const { return this->visible; }
	void setVisible(const bool visible) { this->visible = visible; }

private:
	bool visible = true;
	QVector3D translation = {0,0,0};
};

#endif // I_OPENGLCITYVIEW_H
