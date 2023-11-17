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

//#include "geometryengine.h"	// 3D buildings geometry
//class GeometryEngine;

class OpenGLcityView : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
	using QOpenGLWidget::QOpenGLWidget;
	~OpenGLcityView();

protected:
	//void mousePressEvent(QMouseEvent *e) override;
	//void mouseReleaseEvent(QMouseEvent *e) override;
	//void timerEvent(QTimerEvent *e) override;

	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

	void initShaders();

private:
	//QBasicTimer timer;	// faster than QTimer
	QOpenGLShaderProgram shader;
	//GeometryEngine *geometries = nullptr; // list of buildings

	/*
	 * to class Camera
	 */
	/*
	QMatrix4x4 projection;

	QVector2D mousePressPosition;
	QVector3D rotationAxis;
	qreal angularSpeed = 0;
	QQuaternion rotation;
	*/
};

#endif // OPENGLCITYVIEW_H
