#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera
{
public:
	Camera();

	QMatrix4x4 ComputeMPV();
	QMatrix4x4 getMVP() const { return this->matMVP; };

	void ResizeView(int w, int h);

	float getZoom() const { return this->zoom; }
	void setZoom(const float &zoom, const bool recomputeMVP = true) {
		this->zoom = zoom;
		if (recomputeMVP) this->ComputeMPV();
	}

	QVector3D getPosition() const { return this->position; }
	void setPosition(const QVector3D &pos, const bool recomputeMVP = true) {
		this->position = pos;
		if (recomputeMVP) this->ComputeMPV();
	}

	float getAngleH() const { return this->angleH; }
	void setAngleH(const float &angleH, const bool recomputeMVP = true) {
		this->angleH = angleH;
		if (recomputeMVP) this->ComputeMPV();
	}

	float getAngleV() const { return this->angleV; }
	void setAngleV(const float &angleV, const bool recomputeMVP = true) {
		if (angleV < 0) this->angleV = 0;
		else if (angleV > this->angleVMax) this->angleV = this->angleVMax;
		else this->angleV = angleV;
		if (recomputeMVP) this->ComputeMPV();
	}


private:
	/**
	 * @brief angles (in degrees !)
	 * angleH: angle entre le nord et la projection de la directions de la caméra sur le plan sol (entre -pi et pi)
	 * angleV: angle entre le nadir et le vecteur direction (entre 0 et pi, contraint entre 0 et pi/4)
	 */
	float angleH;
	float angleV;
	float zoom;
	const float angleVMax = 45.;
	QVector3D position;
	QMatrix4x4 matProjection;
	QMatrix4x4 matMVP;
};

#endif // CAMERA_H
