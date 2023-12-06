#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

#include "i_openglcityview.h"

/**
 * @brief The Camera class
 * Camera contains all projection informations to compute 4D matrix from 3D real
 * coordinates to 3D screen coordinates.
 */
class Camera
{
public:

	/**
	 * @brief
	 * initialise a camera with default values
	 *
	 * Position (0,0,20), look at down direction, with north direction at up on screen
	 */
	Camera();

	/**
	 * @brief
	 * Update the matrix MVP (converting 3D real point into 3D screen point)
	 * M is 4D-identity because all buildings are linked
	 *
	 * Also update the MVP_Compass matrix
	 *
	 * @see getMVP
	 * @see getMVPCompass
	 *
	 * @return 4D-matrix (QMatrix4x4) MVP
	 */
	QMatrix4x4 ComputeMPV();

	/**
	 * @brief
	 * Return MVP matrix without recompute it
	 *
	 * @return 4D-matrix (QMatrix4x4) MVP
	 *
	 * @see ComputeMVP
	 */
	QMatrix4x4 getMVP() const { return this->matMVP; };

	/**
	 * @brief
	 * Return MVP matrix for compass (no projection, independent scale and
	 * fixed translation to widget bottom-left corner) without recompute it
	 *
	 * @return 4D-matrix (QMatrix4x4) MVP
	 *
	 * @see ComputeMVP
	 */
	QMatrix4x4 getMVPCompass() const { return this->matMVPCompass; };

	/**
	 * @brief
	 * Update view size
	 *
	 * @param width, height: new size of widget
	 */
	void ResizeView(int width, int height);

	/**
	 * @brief
	 * Getter for zoom value (float)
	 *
	 * @warning Zoom is the scale of the result 3D image in screen, and is different
	 * than z position
	 *
	 * @see move or setPosition to update Z value
	 *
	 * @return zoom
	 */
	float getZoom() const { return this->zoom; }
	/**
	 * @brief setZoom
	 *
	 * Setter for zoom value (float)
	 *
	 * @warning Zoom is the scale of the result 3D image in screen, and is different
	 * than z position
	 *
	 * @param zoom: new value for zoom.
	 * @param recomputeMVP: set to false to don't update MVP.
	 *
	 * @see move or setPosition to update Z value
	 */
	void setZoom(const float &zoom, const bool recomputeMVP = true) {
		this->zoom = zoom;
		if (recomputeMVP) this->ComputeMPV();
	}

	/**
	 * @brief
	 * Getter for position
	 *
	 * @see setZoom to update zoom, move to set(get()+...)
	 *
	 * @return position : QVector3D
	 */
	QVector3D getPosition() const { return this->position; }
	/**
	 * @brief setPosition
	 *
	 * Setter for position
	 *
	 * @param pos: new camera position (QVector3D)
	 * @param recomputeMVP: set to false to don't update MVP.
	 *
	 * @see setZoom: to update zoom
	 * @see move: to translate camera
	 */
	void setPosition(const QVector3D &pos, const bool recomputeMVP = true) {
		this->position = pos;
		if (recomputeMVP) this->ComputeMPV();
	}

	/**
	 * @brief
	 * Getters and setters for camera look at direction in degrees (float, float)
	 *
	 * @param angleH: horizontal angle, between north and screen vector up.
	 * @param angleV: vertical angle, camera nadir and camera look at direction. This
	 * angle is shrinked between 0 and 45°.
	 * @param recomputeMVP: set to false to don't update MVP.
	 *
	 * @return angleH or angleV
	 *
	 * @warning angle units: degrees
	 *
	 * @see getAngleH, setAngleH, getAngleV, setAngleV
	 */
	float getAngleH() const { return this->angleH; }
	/**
	 * @see getAngleH
	 */
	void setAngleH(const float &angleH, const bool recomputeMVP = true) {
		this->angleH = angleH;
		if (recomputeMVP) this->ComputeMPV();
	}

	/**
	 * @see getAngleH
	 */
	float getAngleV() const { return this->angleV; }
	/**
	 * @see getAngleH
	 */
	void setAngleV(const float &angleV, const bool recomputeMVP = true) {
		if (angleV < 0) this->angleV = 0;
		else if (angleV > this->angleVMax) this->angleV = this->angleVMax;
		else this->angleV = angleV;
		if (recomputeMVP) this->ComputeMPV();
	}

	/**
	 * @brief move
	 *
	 * translate the camera
	 *
	 * @see setPosition
	 * @param v: translation vector 3D
	 */
	void move(const QVector3D & v) {
		this->setPosition(this->position += v);
	}

	/**
	 * @brief turn
	 *
	 * rotate the camera
	 *
	 * @see setAngleH, setAngleV
	 * @param angleH: horizontal rotation
	 * @param angleV: vertical rotation
	 */
	void turn(const float angleH, const float angleV) {
		this->setAngleH(this->angleH + angleH, false);
		this->setAngleV(this->angleV + angleV);
	}

	/**
	 * @brief getPxRatio
	 *
	 * get the ratio between px and screen units (like rescaling factor)
	 *
	 * @return pxRatio (float)
	 */
	float getPxRatio() const {
		return this->pxRatio;
	}

	/**
	 * @brief Picking3D convert a 2D position in screen to a 3D segment in real coordinates
	 * @param posScreen: position of point in screen (px coordinates)
	 * @param p1 [out], p2 [out]: points that define the segment. First point is the
	 *	closest to camera
	 *
	 * @see ProjToScreen, ProjFromScreen
	 */
	void Picking3D(const QPoint & posScreen, QVector3D & p1, QVector3D &p2) const;

	/**
	 * @brief ProjToScreen convert 3D real coordinates into 3D screen coordinates
	 *
	 * z screen coordinate corresponds to background (z=-1) / foreground (z=+1) level
	 *
	 * @param pos: real position (in dataset unit [meters])
	 * @return screen position (in px)
	 *
	 * @see ProjFromScreen, Picking3D
	 */
	QVector3D ProjToScreen(const QVector3D & pos) const;
	/**
	 * @brief ProjFromScreen convert 3D screen coordinates into 3D real coordinates
	 *
	 * z screen coordinate corresponds to background (z=-1) / foreground (z=+1) level
	 *
	 * @param pos: screen position (in px)
	 * @return real position (in dataset unit [meters])
	 *
	 * @see ProjToScreen, Picking3D
	 */
	QVector3D ProjFromScreen(const QVector3D & pos) const;

	/**
	 * @brief getEmprise give the 3D box of the view of the camera
	 *
	 * @see Emprise
	 *
	 * @return camera's emprise
	 */
	Emprise getEmprise() const { return this->emprise; };

	/**
	 * @brief Return true if emprise has changed since last 'consumeChanges' call
	 *
	 * @return true if screen refresh is needed
	 */
	bool ConsumeChanges() {
		bool l = this->hasChanged;
		this->hasChanged = false;
		return l;
	}

private:
	/**
	 * @brief angles (in degrees !)
	 * angleH: angle between north and the projection of the camera direction on the
	 *  ground plane (between -pi and pi)
	 * angleV: angle between the nadir and the direction vector (between 0 and pi,
	 * constrained between 0 and pi/4)
	 */
	float angleH;
	float angleV;
	float zoom;
	const float angleVMax = 45.;
	float pxRatio;
	float wHeight, wWidth;
	QVector3D position;
	QMatrix4x4 matProjection;
	QMatrix4x4 matMVP;
	QMatrix4x4 matMVPCompass;
	Emprise emprise;
	bool hasChanged = true;
};

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

#endif // CAMERA_H
