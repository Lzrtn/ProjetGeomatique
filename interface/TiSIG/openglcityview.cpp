#include "openglcityview.h"

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

	timer.start(15, this); // run this->timerEvent every n msec
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

void OpenGLcityView::timerEvent(QTimerEvent *e)
{
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
