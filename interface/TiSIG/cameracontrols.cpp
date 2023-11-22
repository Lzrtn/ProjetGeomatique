#include "cameracontrols.h"
#include <iostream>

#include <QKeyEvent>
#include <cmath>

CameraControls::CameraControls()
{
	this->reset();
}

bool CameraControls::update(const float dt)
{
	if (this->camera == nullptr) return false;

	if (this->keysPressed["ctrl"]) {
		if (this->keysPressed["left"])	this->camera->turn(-1,0);
		if (this->keysPressed["right"])	this->camera->turn(1,0);
		if (this->keysPressed["up"])	this->camera->turn(0,1);
		if (this->keysPressed["down"])	this->camera->turn(0,-1);
	} else {
		float c = this->speedNav2D * cos(this->camera->getAngleH() * M_PI/180);
		float s = this->speedNav2D * sin(this->camera->getAngleH() * M_PI/180);
		if (this->keysPressed["left"])	this->camera->move({-dt*c,-dt*s, 0});
		if (this->keysPressed["right"])	this->camera->move({ dt*c, dt*s, 0});
		if (this->keysPressed["up"])	this->camera->move({-dt*s, dt*c, 0});
		if (this->keysPressed["down"])	this->camera->move({ dt*s,-dt*c, 0});
	}
	if (this->keysPressed["shift"])	this->camera->move({ 0, 0, dt * this->speedNavZ});
	if (this->keysPressed["space"])	this->camera->move({ 0, 0,-dt * this->speedNavZ});

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
	case Qt::Key_Control:
		this->keysPressed["ctrl"] = pressed;
		break;
	default:
		break;
	}

}

void CameraControls::reset()
{
	for (auto &pair: this->keysPressed)
		pair.second = false;
}
