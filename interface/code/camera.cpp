#include "camera.h"

Camera::Camera()
{

}

QMatrix4x4 Camera::ComputeMPV()
{
	float w = 1, h = 1;
	QMatrix4x4 projection;
	qreal aspect_ratio = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

	// Reset and compute projection
	projection.setToIdentity();
	projection.perspective(fov, aspect_ratio, zNear, zFar);

	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	matrix.rotate(1, {0,0,0});
	matrix.scale(0.5);

	// Set modelview-projection matrix
	return projection * matrix;
}
