#include "openglcityview.h"

OpenGLcityView::~OpenGLcityView()
{
	// Make sure the context is current when deleting the texture
	// and the buffers.

	makeCurrent();
	//delete geometries;
	doneCurrent();
}
#include <QMouseEvent>

#include <cmath>

void OpenGLcityView::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0, 0, 0, 1);
	initShaders();

	// Add object
	std::vector<QVector3D> p = {
		{ 2, 1,-1}, { 4, 1, 1}, { 4,-1,-1},
		{ 2, 1,-1}, { 4, 1, 1}, { 2,-1, 1},
		{ 4,-1,-1}, { 4, 1, 1}, { 2,-1, 1},
		{ 4,-1,-1}, { 2, 1,-1}, { 2,-1, 1}
	};
	std::vector<QVector3D> n = p;
	std::vector<QVector2D> t = {
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1}
	};
	//geometries = new GeometryEngine(p, n, t, ":/cube.png");

	//timer.start(12, this); // run this->timerEvent every n msec
}

void OpenGLcityView::initShaders()
{
	// Compile vertex shader
	if (!shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
		close();

	// Compile fragment shader
	if (!shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
		close();

	// Link shader pipeline
	if (!shader.link())
		close();

	// Bind shader pipeline for use
	if (!shader.bind())
		close();
}

void OpenGLcityView::resizeGL(int w, int h)
{
	// Calculate aspect ratio
	//qreal aspect_ratio = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	//const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

	// Reset and compute projection
	//projection.setToIdentity();
	//projection.perspective(fov, aspect_ratio, zNear, zFar);
}

void OpenGLcityView::paintGL()
{
	// Clear color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth buffer and back face culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	shader.bind();

	// TODO: use a Camera class to manage motion events and mvp matrix
	// Calculate model view transformation
	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	//matrix.rotate(rotation);
	matrix.scale(0.5);

	// Set modelview-projection matrix
	//program.setUniformValue("mvp_matrix", projection * matrix);

	// Draw geometry
	//geometries->draw(&program);
}
