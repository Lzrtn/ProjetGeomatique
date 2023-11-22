#ifndef OPENGLCITYVIEW_H
#define OPENGLCITYVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "building3d.h"
#include "camera.h"
#include "cameracontrols.h"

#include <map>

class OpenGLcityView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	using QOpenGLWidget::QOpenGLWidget;
	~OpenGLcityView();

	/**
	 * @brief Add a building to display it
	 * Buildings are stored with their id
	 *
	 * @param id
	 * @param buildingFactory
	 */
	void AddBuilding(const int id, const Building3DFactory &buildingFactory);

	/**
	 * @brief Remove a building and free memory
	 *
	 * @param id
	 */
	void DeleteBuilding(const int id);

protected:

	/**
	 * @brief these overrided methodes are used by QT to init and display the widget
	 */
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void setVisible(bool visible) override;

	/**
	 * @brief InitShaders load and init shader OpenGL program
	 */
	void InitShaders();


	void timerEvent(QTimerEvent *e) override;


private:
	QOpenGLShaderProgram shader;
	std::map<int, Building3D*> buildings;

	// TODO: turn this function in a Camera class and add controls
	QMatrix4x4 projection;
	Camera camera;
	CameraControls controls;

	QBasicTimer timer;
	float lastTimeUpdate;
	std::chrono::steady_clock::time_point timeStart;
};

#endif // OPENGLCITYVIEW_H
