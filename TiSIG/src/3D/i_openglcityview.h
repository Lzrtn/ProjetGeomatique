#ifndef I_OPENGLCITYVIEW_H
#define I_OPENGLCITYVIEW_H

#include "building3d.h"
#include "object3d.h"
#include <map>


/**
 * @brief The ICameraDisplayInfo class give function to display camera values
 * like position, orientation and zoom
 */
class ICameraDisplayInfo {
public:

	/**
	 * @brief Display3DCameraCoordinates is called when camera position has changed
	 * Override then do action with this new value
	 *
	 * @param camPosition:	new position
	 */
	virtual void Display3DCameraCoordinates(QVector3D camPosition) = 0;

	/**
	 * @brief Display3DZoomLevel is called when camera zoom level has changed
	 * Override then do action with this new value
	 *
	 * @param zoom:	new zoom
	 */
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
	/**
	 * @brief 3D coordinates for volume
	 */
	QVector3D a, b, c, d, e, f, g, h;

	/**
	 * @brief 3D coordinates for ground plan (z=0)
	 */
	QVector3D g_a, g_b, g_c, g_d;

	/**
	 * @brief updateGroundCoordinates compute g_a, g_b, g_c, and g_d with other coordinates
	 */
	void updateGroundCoordinates() {
		// intersection between line (a,e) [resp. (b,f); (c,g); (e,h)] and plane (z=0)
		// if no intersect, first point will be chosen
		g_a = e.z() != a.z() ? a - a.z() * (e-a) / (e.z() - a.z()) : a;
		g_b = f.z() != b.z() ? b - b.z() * (f-b) / (f.z() - b.z()) : b;
		g_c = g.z() != c.z() ? c - c.z() * (g-c) / (g.z() - c.z()) : c;
		g_d = h.z() != d.z() ? d - d.z() * (h-d) / (h.z() - d.z()) : d;
	}

	/**
	 * @brief operator !=
	 * test equality between two emprises
	 *
	 * @return true if all coordinates are same
	 */
	bool operator!= (const Emprise & o) const {
		return !(a==o.a && b==o.b && c==o.c && d==o.d
			  && e==o.e && f==o.f && g==o.g && h==o.h);
	}

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

	/**
	 * @brief PickingObjectInfo
	 *
	 * Get attributes of the first object that intersects the segment [p1 p2], closest than p1
	 *
	 * @param p1:		[in]	first point of the segment
	 * @param p2:		[in]	second point of the segment
	 * @param idObject:	[out]	id of object (unused if return false)
	 * @param data:		[out]	attributar table, link key-value 'unused if return false)
	 * @return:			bool	true if an object is found
	 */
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

	/**
	 * @brief getCameraInitPosition
	 *
	 * Use same coordinate than data
	 *
	 * @return camera position when click on "center on emprise". Orientaion is 0,0
	 */
	QVector3D getCameraInitPosition() const { return this->cameraInitPosition; }

protected:
	/**
	 * @brief getTranslation
	 *
	 * Coordinates are defined with a translation. Set this translation.
	 */
	void setTranslation(const QVector3D & translation) { this->translation = translation; }

	/**
	 * @brief setCameraInitPosition
	 *
	 * Use same coordinate than data
	 *
	 * set camera position when click on "center on emprise". Orientaion is 0,0
	 */
	void setCameraInitPosition(const QVector3D & position) { this->cameraInitPosition = position; }

private:
	QVector3D translation = {0,0,0};
	QVector3D cameraInitPosition = {0, 0, 50};
};


/**
 * @brief The Updatable class make objects can be requested for update
 *
 * example
 *
 * Obj myObj; // class Obj extends Updatable
 *
 * // do some action
 *
 * if (myObj.ConsumeUpdate()) {
 *	 // do some action
 * }
 */
class Updatable {
public:
	/**
	 * @brief RequestUpdate
	 * Next ConsumeUpdate will be true
	 *
	 * @see ConsumeUpdate
	 */
	void RequestUpdate() {
		this->requestedUpdate = true;
	}

	/**
	 * @brief Return true if emprise has changed since last 'ConsumeUpdate' call
	 *
	 * @return true if update is requested
	 *
	 * @see RequestUpdate
	 */
	bool ConsumeUpdate() {
		if (this->requestedUpdate) {
			this->requestedUpdate = false;
			return true;
		}
		return false;
	}

private:
	bool requestedUpdate = false;
};

#endif // I_OPENGLCITYVIEW_H
