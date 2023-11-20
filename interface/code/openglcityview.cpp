#include "openglcityview.h"

#include <QMouseEvent>

#include <cmath>

#include <iostream>

OpenGLcityView::~OpenGLcityView()
{
	// Make sure the context is current when deleting the texture
	// and the buffers.

	this->makeCurrent();
	delete this->building;
	this->doneCurrent();
}

void OpenGLcityView::initializeGL()
{
	this->initializeOpenGLFunctions();
	this->glClearColor(0, 0, 0, 1);
	this->initShaders();

	// ajout d'un batiment de test
	//this->buildings[6] = new Building3D();
	this->addBuilding(5, Building3DFactory(0));
	this->addBuilding(156, Building3DFactory(1));

	//timer.start(12, this); // run this->timerEvent every n msec
}

void OpenGLcityView::initShaders()
{
	// Compile vertex shader
	if (!this->shader.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
		this->close();

	// Compile fragment shader
	if (!this->shader.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
		this->close();

	// Link shader pipeline
	if (!this->shader.link())
		this->close();

	// Bind shader pipeline for use
	if (!this->shader.bind())
		this->close();
}

void OpenGLcityView::addBuilding(const int id, const Building3DFactory &buildingFactory)
{
	if (this->buildings.find(id) != this->buildings.end())
		this->deleteBuilding(id);

	this->buildings[id] = buildingFactory.NewBuilding();
}
void OpenGLcityView::deleteBuilding(const int id)
{
	delete this->buildings[id];
	this->buildings.erase(id);
}

void OpenGLcityView::resizeGL(int w, int h)
{
	// Calculate aspect ratio
	qreal aspect_ratio = qreal(w) / qreal(h ? h : 1);

	// Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
	const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

	// Reset and compute projection
	this->projection.setToIdentity();
	this->projection.perspective(fov, aspect_ratio, zNear, zFar);
}

void OpenGLcityView::paintGL()
{
	// Clear color and depth buffer
	this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth buffer and back face culling
	this->glEnable(GL_DEPTH_TEST);
	this->glEnable(GL_CULL_FACE);

	this->shader.bind();

	// TODO: use a Camera class to manage motion events and mvp matrix
	// Calculate model view transformation
	QMatrix4x4 matrix;
	matrix.translate(0.0, 0.0, -5.0);
	//matrix.rotate(rotation);
	matrix.scale(0.5);

	// Set modelview-projection matrix
	this->shader.setUniformValue("mvp_matrix", this->projection * matrix);

	// Draw geometry
	for (auto &pair : this->buildings) {
		std::cout << pair.first << "\n";
		pair.second->draw(&this->shader);
	}
}
