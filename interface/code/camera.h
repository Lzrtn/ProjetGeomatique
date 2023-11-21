#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
//#include <QQuaternion>
//#include <QVector2D>

class Camera
{
public:
	Camera();

	QMatrix4x4 getMVP() const {
		return this->matrixMVP;
	};
	QMatrix4x4 ComputeMPV();

	void setAngleH(const float & angleH) {
		this->angleH = angleH;
	}
	void setAngleV(const float & angleV) {
		this->angleV = angleV;
	}

private:
	float angleH;
	float angleV;
	QVector3D position;
	QMatrix4x4 matrixMVP;
};

#endif // CAMERA_H
