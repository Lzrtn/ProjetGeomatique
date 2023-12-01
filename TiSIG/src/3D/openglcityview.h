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
#include "mnt3d.h"
#include "camera.h"
#include "cameracontrols.h"
#include "compass.h"
#include "i_openglcityview.h"

#include <map>

/**
 * @brief The OpenGLcityView class is a widget that can display buildings in 3D
 */
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

	void setCamInfoDisplayer(ICameraDisplayInfo * camInfoDisplayer) {
		this->camInfoDisplayer = camInfoDisplayer;
	};

	/**
	 * @brief linkBuildingStorage
	 * link to a building storage object, to load buildings
	 *
	 * @param buildingStorage
	 */
	void linkBuildingStorage(OpenGLCityView_BuildingStorage * buildingStorage);

protected:

	ICameraDisplayInfo * camInfoDisplayer;

	/**
	 * @brief these overrided methodes are used by QT to init and display the widget
	 */
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void setVisible(bool visible) override;
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;


	/**
	 * @brief InitShaders load and init shader OpenGL program
	 */
	void InitShaders();

	void timerEvent(QTimerEvent* /*e*/) override; // remove parameter name because unused (disable warning)

private:

	/**
	 * @brief UpdateBuildings
	 * Load buildings from buildingsStorage
	 */
	void UpdateBuildings();

	QOpenGLShaderProgram shader;
	std::map<int, Object3D*> buildings;
	Object3D * compass = nullptr;

	// TODO: turn this function in a Camera class and add controls
	QMatrix4x4 projection;
	Camera camera;
	CameraControls controls;

	QBasicTimer timer;
	float lastTimeUpdate;
	std::chrono::steady_clock::time_point timeStart;
	const int timerDuration = 15; // in msec

	OpenGLCityView_BuildingStorage * buildingStorage = nullptr;
};

#endif // OPENGLCITYVIEW_H
