#include "camera.h"

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

	this->matMVPCompass.setToIdentity();
	this->matMVPCompass.translate(1, -1, 0);		// center on bottom-right corner
	this->matMVPCompass.scale(100/this->wWidth, 100/this->wHeight, -0.1);	// scale to square ratio, with constant pxRatio
	this->matMVPCompass.translate(-1, 1, 0);		// set center of compass arround the corner (all visible)
	this->matMVPCompass.rotate(this->angleV*1.3, {-1, 0, 0});	// apply rotation
	this->matMVPCompass.rotate(this->angleH, {0, 0, -1});

	// matModel = identity
	this->matMVP = this->matProjection * matView;
	return this->getMVP();
}

void Camera::ResizeView(int width, int height)
{
	this->pxRatio = (width < height ? width : height);
	this->wWidth = width;
	this->wHeight = height;
	qreal aspect_ratio = qreal(width) / qreal(height ? height : 1);

	const qreal zNear = 3.0, zFar = 1000, fov = 45.0;
	this->matProjection.setToIdentity();
	this->matProjection.perspective(fov, aspect_ratio, zNear, zFar);
}
