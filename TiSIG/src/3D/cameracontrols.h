#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

#include "camera.h"

#include <QOpenGLWidget>

/**
 * @brief The CameraControls class will link keyboard and mouse events to camera movment
 *
 * @warning Before to use, call setCamera to init control
 *
 * Bind manualy events:
 * - keyPressEvent
 * - mousePressEvent
 * - mouseMoveEvent
 * - wheelEvent
 */
class CameraControls
{
public:
	/**
	 * @brief Constructor
	 *
	 * @warning call setCamera before to use
	 *
	 * @see setCamera
	 */
	CameraControls();

	/**
	 * @brief
	 * Set the camera that will accept movment
	 *
	 * @param camera
	 */
	void setCamera(Camera *camera) { this->camera = camera; };

	/**
	 * @brief update camera movment (from keyboard actions)
	 *
	 * @param dt: delta time from the last update
	 * @return retrun will be true if camera has changed (need to update view)
	 *
	 * @see reset
	 */
	bool update(const float dt);

	/**
	 * @brief reset keys pressed
	 *
	 * Reset memory of keys pressed
	 *
	 * When a key is pressed, it's keeped in memory. To prevent case release event
	 * can't be catched (for example if widget is hidden), call this method when widget
	 * is set to visible
	 *
	 * @see keyPressEvent
	 */
	void reset();

	/**
	 * @brief link event keyboard
	 * @param event
	 * @param pressed: is key pressed (true) or released (false) ?
	 *
	 * @see reset
	 */
	void keyPressEvent(QKeyEvent *event, bool pressed);

	/**
	 * @brief link event mouse click
	 * @param event
	 * @param pressed: is mouse buttun pressed (true) or released (false) ?
	 */
	void mousePressEvent(QMouseEvent *event, bool pressed);

	/**
	 * @brief link event mouse motion
	 * @param event
	 */
	void mouseMoveEvent(QMouseEvent *event);

	/**
	 * @brief link event mouse wheel
	 * @param event
	 */
	void wheelEvent(QWheelEvent *event);

	/**
	 * @brief ZoomIn change zoom of camera
	 *
	 * @param zoomIn:	true->zoomIn; false->zoomOut
	 */
	void ZoomIn(bool zoomIn = true);

private:

	Camera * camera;

	std::map<std::string, bool> keysPressed;

	float speedNav2D	= 0.008;
	float speedNavZ		= 0.100;
	float speedRot		= 0.001;
	float speedZoom		= 0.25;
	float speedMouseMove= 150;
	float speedMouseRot	= 50;
	float speedMouseZ	= 50; // metres par cran de mollete
	QPoint lastPosClick;
};

#endif // CAMERACONTROLS_H
