#include "openglcityview.h"

#include <iostream>
#include <QKeyEvent>

OpenGLcityView::~OpenGLcityView()
{
	// Make sure the context is current when deleting the texture,
	// building and buffers.

	this->makeCurrent();
	/*
	for (auto &pair : this->buildings) {
		this->DeleteBuilding(pair.first);
	}
	*/
	delete this->compass;

	this->doneCurrent();
}

void OpenGLcityView::initializeGL()
{
	this->initializeOpenGLFunctions();
//	this->glClearColor(0, 0.05, 0.3, 1);
//	this->glClearColor(7./256, 31./256, 50./256, 1); // Paris logo color
//	this->glClearColor(0.012, 0.369, 0.647, 1); // blue logo Lyon
//	this->glClearColor(0.894, 0.212, 0.176, 1); // red logo Lyon
	this->glClearColor(0.87, 0.87, 0.92, 1); // blue-gray-light
	this->InitShaders();

	/*
	// ajout d'un batiment de test
	this->AddBuilding(5, Building3DFactory(0));
	this->AddBuilding(156, Building3DFactory(1));
	this->AddBuilding(4561, Building3DFactory(2));
	this->AddBuilding(0, Building3DFactory(3));
	this->AddBuilding(1, Building3DFactory(5));

	this->buildings[86] = MNT3DFactory(5).New();
	*/
	this->compass = CompassFactory().New();
	this->camera.setAngleV(0);

	this->controls.setCamera(&this->camera);
	//this->camera.setPosition({0, 0, 400});
}

void OpenGLcityView::InitShaders()
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

void OpenGLcityView::timerEvent(QTimerEvent* /*e*/)
{
	float currentTime = (std::chrono::steady_clock::now() - this->timeStart).count() / 1e6;
	float dt = currentTime - lastTimeUpdate;
	lastTimeUpdate = currentTime;
	// if dt is too hire than timerDuration (stop and restart)
	if (dt > this->timerDuration * 2)
		dt = this->timerDuration * 2;
	if (this->controls.update(dt) && this->isValid())
		this->camera.ComputeMPV();
	if (this->camera.consumeChanges()) {
		this->UpdateBuildings();
		this->update();
	}
}

void OpenGLcityView::UpdateBuildings()
{
	Emprise emprise = this->camera.getEmprise();
	for (auto pair: this->layers) {
		pair.second->UpdateEmprise(emprise);
	}
}

void OpenGLcityView::resizeGL(int w, int h)
{
	this->camera.ResizeView(w, h);
}

void OpenGLcityView::paintGL()
{
	// Clear color and depth buffer
	this->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Enable depth buffer and back face culling
	this->glEnable(GL_DEPTH_TEST);
	this->glEnable(GL_CULL_FACE);

	this->shader.bind();

	if (this->camInfoDisplayer != nullptr) {
		this->camInfoDisplayer->Display3DCameraCoordinates(this->camera.getPosition());
		this->camInfoDisplayer->Display3DZoomLevel(this->camera.getZoom());
	}

	this->shader.setUniformValue("mvp_matrix", this->camera.getMVPCompass());
	this->shader.setUniformValue("power_light", GLfloat(0.5));
	this->shader.setUniformValue("translation", QVector3D(0,0,0));
	this->compass->Draw(&this->shader);

	// Set modelview-projection matrix
	this->shader.setUniformValue("mvp_matrix", this->camera.getMVP());
	this->shader.setUniformValue("power_light", GLfloat(1.0));

	// Draw layers
	/*
	for (auto &pair : this->buildings) {
		pair.second->Draw(&this->shader);
	}*/

	for (auto &pair : this->layers) {
		pair.second->Draw(&this->shader);
	}
}


void OpenGLcityView::setVisible(bool visible)
{
	this->QOpenGLWidget::setVisible(visible);
	if (visible)
	{
		this->timeStart = std::chrono::steady_clock::now();
		this->lastTimeUpdate = 0;
		timer.start(this->timerDuration, this); // run this->timerEvent every n msec
		this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
		this->setFocus();
		this->controls.reset();
	}
	else
		timer.stop();
}


///////////////////////////  bind events  /////////////////////////////////////////////
void OpenGLcityView::keyPressEvent(QKeyEvent *event)
{
	this->controls.keyPressEvent(event, true);
}
void OpenGLcityView::keyReleaseEvent(QKeyEvent *event)
{
	this->controls.keyPressEvent(event, false);
}
void OpenGLcityView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton)
		this->controls.mousePressEvent(event, true);
	if (event->button() & Qt::RightButton) {

	}
}
void OpenGLcityView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton)
		this->controls.mousePressEvent(event, false);
	if (event->button() & Qt::RightButton) {

	}
}
void OpenGLcityView::mouseMoveEvent(QMouseEvent *event)
{
	this->controls.mouseMoveEvent(event);
}
void OpenGLcityView::wheelEvent(QWheelEvent *event)
{
	this->controls.wheelEvent(event);
}
