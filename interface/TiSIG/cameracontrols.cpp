#include "cameracontrols.h"
#include <iostream>

#include <QKeyEvent>

CameraControls::CameraControls()
{
	this->reset();
}

bool CameraControls::update(const float dt)
{
	if (this->camera == nullptr) return false;

	if (this->keysPressed["left"])	this->camera->move({-dt * this->speedNav2D, 0, 0});
	if (this->keysPressed["right"])	this->camera->move({ dt * this->speedNav2D, 0, 0});
	if (this->keysPressed["up"])	this->camera->move({0, dt * this->speedNav2D, 0});
	if (this->keysPressed["down"])	this->camera->move({0,-dt * this->speedNav2D, 0});
	if (this->keysPressed["shift"])	this->camera->move({0, 0, dt * this->speedNav2D});
	if (this->keysPressed["space"])	this->camera->move({0, 0,-dt * this->speedNav2D});

	return true;
}

void CameraControls::keyPressEvent(QKeyEvent *event, bool pressed)
{
	switch(event->key()) {
	case Qt::Key_Left:
		this->keysPressed["left"] = pressed;
		break;
	case Qt::Key_Right:
		this->keysPressed["right"] = pressed;
		break;
	case Qt::Key_Up:
		this->keysPressed["up"] = pressed;
		break;
	case Qt::Key_Down:
		this->keysPressed["down"] = pressed;
		break;
	case Qt::Key_Space:
		this->keysPressed["space"] = pressed;
		break;
	case Qt::Key_Shift:
		this->keysPressed["shift"] = pressed;
		break;
	default:
		break;
	}

}

void CameraControls::reset()
{
	this->keysPressed = {
		{"left", 0},
		{"right",0},
		{"up",   0},
		{"down", 0},
	};
}
