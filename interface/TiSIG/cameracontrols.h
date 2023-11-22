#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include "camera.h"

#include <QOpenGLWidget>

class CameraControls
{
public:
	CameraControls();

	void setCamera(Camera *camera) { this->camera = camera; };

	bool update(const float dt);

	void keyPressEvent(QKeyEvent *e, bool pressed);
	void mousePressEvent(QMouseEvent *event, bool pressed);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

	void reset();

private:

	Camera * camera;

	std::map<std::string, bool> keysPressed;

	float speedNav2D	= 0.008;
	float speedNavZ		= 0.010;
	float speedRot		= 0.001;
	float speedZoom		= 0.25;
	QPoint lastPosClick;
};

#endif // CAMERACONTROLS_H
