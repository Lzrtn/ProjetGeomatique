#include "camera.h"

#include <iostream>
#include <cmath>

Camera::Camera() :
	angleH(0),
	angleV(45),
	zoom(2),
	position(0,0,20)
{
	this->matMVP.setToIdentity();
	this->ResizeView(1, 1); // square ratio
}

QMatrix4x4 Camera::ComputeMPV()
{
	QMatrix4x4 matView;
	matView.setToIdentity();
	matView.scale(this->zoom, this->zoom, 1);
	matView.rotate(this->angleV, {-1,0,0});
	matView.rotate(this->angleH, {0,0,-1});
	matView.translate(-this->position);

	std::cout << "camera:\tpos(" << this->position.x() << " " << this->position.y() << " " << this->position.z() << ")"
			  << "\tzoom " << this->zoom << "\tangleH " << this->angleH << "\tangleV " << this->angleV << std::endl;

	// matModel = identity
	this->matMVP = this->matProjection * matView;
	return this->getMVP();
}

void Camera::ResizeView(int w, int h)
{
	qreal aspect_ratio = qreal(w) / qreal(h ? h : 1);

	const qreal zNear = 3.0, zFar = 1000, fov = 45.0;
	this->matProjection.setToIdentity();
	this->matProjection.perspective(fov, aspect_ratio, zNear, zFar);
}
