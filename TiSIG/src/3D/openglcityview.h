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
#include "layer3d.h"

// cyclic dependency: use forward declaration and include in .cpp
//#include "../interface/layermanager3d.h"
class LayerManager3D;	// forward declaration

#include <map>

/**
 * @brief The OpenGLcityView class is a widget that can display buildings in 3D
 */
class OpenGLcityView : public QOpenGLWidget, protected QOpenGLFunctions, public Updatable
{
	Q_OBJECT
public:
	using QOpenGLWidget::QOpenGLWidget;
	~OpenGLcityView();

	void setCamInfoDisplayer(ICameraDisplayInfo * camInfoDisplayer) {
		this->camInfoDisplayer = camInfoDisplayer;
	};

	void setPickingInfoDisplayer(IPicking3DDisplayInfo * pickingInfoDisplayer) {
		this->pickingInfoDisplayer = pickingInfoDisplayer;
	};

	void setLayerManager(LayerManager3D * layerManager) {
		this->layerManager = layerManager;
	}

	void addLayer(const int id, Layer3D * layer) {
		this->layers[id] = layer;
	}

	Layer3D* getLayer(const int id) const { return this->layers.at(id); }
	void removeLayer(const int id) { this->layers.erase(id); }

	void ZoomIn() { this->controls.ZoomIn(true); }
	void ZoomOut() { this->controls.ZoomIn(false); }

	float getSymbologyOpacity() const { return this->symbologyOpacity; }
	void setSymbologyOpacity(const float & value) {
		this->symbologyOpacity = value;
		this->RequestUpdate();
	}

	void ZoomAtEmprise();


protected:

	ICameraDisplayInfo * camInfoDisplayer = nullptr;
	IPicking3DDisplayInfo * pickingInfoDisplayer = nullptr;

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

	Layer3D* getSelectedLayer() const;

	QOpenGLShaderProgram shader;
	std::map<int, Layer3D*> layers;
	//std::map<int, Object3D*> buildings;
	Object3D * compass = nullptr;

	// TODO: turn this function in a Camera class and add controls
	QMatrix4x4 projection;
	Camera camera;
	CameraControls controls;

	QBasicTimer timer;
	float lastTimeUpdate;
	std::chrono::steady_clock::time_point timeStart;
	const int timerDuration = 15; // in msec

	float symbologyOpacity = 0.5;

	LayerManager3D * layerManager = nullptr;

	//OpenGLCityView_BuildingStorage * buildingStorage = nullptr;
};

#endif // OPENGLCITYVIEW_H
