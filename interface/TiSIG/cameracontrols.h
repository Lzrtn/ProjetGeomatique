#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include "camera.h"

class CameraControls
{
public:
	CameraControls();

	void setCamera( Camera * camera ) { this->camera = camera; };
	void setQWidget(QWidget * widget) { this->widget = widget; };

	void update(const float dt);

private:

	Camera * camera;
	QWidget * widget;
};

#endif // CAMERACONTROLS_H
