#include "cameracontrols.h"
#include <iostream>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>

#include <cmath>

CameraControls::CameraControls()
{
	this->reset();
}


void CameraControls::reset()
{
	for (auto &pair: this->keysPressed)
		pair.second = false;
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
		float c = this->speedNav2D / this->camera->getZoom() * cos(this->camera->getAngleH() * M_PI/180);
		float s = this->speedNav2D / this->camera->getZoom() * sin(this->camera->getAngleH() * M_PI/180);
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
	event->accept();
}

void CameraControls::mousePressEvent(QMouseEvent * event, bool /*pressed*/)
{
	this->lastPosClick = event->pos();//{event->x(), event->y()};
}

void CameraControls::mouseMoveEvent(QMouseEvent *event)
{
	QPoint delta = this->lastPosClick - event->pos();

	//std::cout << "move" //<< "\tx:" << event->pos().x() << "\ty:" << event->pos().y()
		//	  << "\tdx:" << delta.x() << "\tdy:" << delta.y() << std::endl;

	if (this->keysPressed["ctrl"])
		this->camera->turn(
				-2*this->speedMouseRot * delta.x() / this->camera->getPxRatio(),
				-this->speedMouseRot * delta.y() / this->camera->getPxRatio());
	else
		this->camera->move(
					{this->speedMouseMove * delta.x() / this->camera->getZoom() / this->camera->getPxRatio(),
					 -this->speedMouseMove * delta.y() / this->camera->getZoom() / this->camera->getPxRatio(),
					 0}
					);

	this->lastPosClick = event->pos();
	event->accept();
}

void CameraControls::wheelEvent(QWheelEvent *event)
{
	if (this->keysPressed["ctrl"])
		this->camera->setZoom(this->camera->getZoom() * pow(2, event->angleDelta().y()/120 * this->speedZoom));
	else
		this->camera->move({0, 0, this->speedMouseZ * event->angleDelta().y() / 120});
	event->accept();
}
