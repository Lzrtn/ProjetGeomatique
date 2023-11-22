#include "openglcityview.h"

#include <iostream>
#include <QKeyEvent>

OpenGLcityView::~OpenGLcityView()
{
	// Make sure the context is current when deleting the texture,
	// building and buffers.

	this->makeCurrent();
	for (auto &pair : this->buildings) {
		this->DeleteBuilding(pair.first);
	}
	this->doneCurrent();
}

void OpenGLcityView::initializeGL()
{
	this->initializeOpenGLFunctions();
	this->glClearColor(0, 0.05, 0.3, 1);
	this->InitShaders();

	// ajout d'un batiment de test
	//this->buildings[6] = new Building3D();
	this->AddBuilding(5, Building3DFactory(0));
	this->AddBuilding(156, Building3DFactory(1));
	this->AddBuilding(4561, Building3DFactory(2));
	this->AddBuilding(0, Building3DFactory(3));

	this->camera.setAngleV(0);

	this->controls.setCamera(&this->camera);
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
		dt = 0 * this->timerDuration * 2;
	if (this->controls.update(dt) && this->isValid())
		this->update();
}

void OpenGLcityView::AddBuilding(const int id, const Building3DFactory &buildingFactory)
{
	if (this->buildings.find(id) != this->buildings.end())
		this->DeleteBuilding(id);

	this->buildings[id] = buildingFactory.NewBuilding();
}
void OpenGLcityView::DeleteBuilding(const int id)
{
	delete this->buildings[id];
	this->buildings.erase(id);
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

	this->camera.ComputeMPV();

	// Set modelview-projection matrix
	this->shader.setUniformValue("mvp_matrix", this->camera.getMVP());

	// Draw geometry
	for (auto &pair : this->buildings) {
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
	this->controls.mousePressEvent(event, true);
}
void OpenGLcityView::mouseReleaseEvent(QMouseEvent *event)
{
	this->controls.mousePressEvent(event, false);
}
void OpenGLcityView::mouseMoveEvent(QMouseEvent *event)
{
	this->controls.mouseMoveEvent(event);
}
void OpenGLcityView::wheelEvent(QWheelEvent *event)
{
	this->controls.wheelEvent(event);
}
