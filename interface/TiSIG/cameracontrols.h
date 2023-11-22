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

	void reset();

private:

	Camera * camera;

	std::map<std::string, bool> keysPressed;

	float speedNav2D	= 0.008;
	float speedNavZ		= 0.010;
	float speedRot		= 0.001;

private slots:
	void mousePressEvent(QMouseEvent *e);	// same function as in qopenglwidget. Use same name (camelCase)
	void mouseReleaseEvent(QMouseEvent *e); // idem
};

#endif // CAMERACONTROLS_H
