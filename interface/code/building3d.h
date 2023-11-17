#ifndef BUILDING3D_H
#define BUILDING3D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <vector>
#include <string>

/**
 * @brief The Building3D class
 * Object that contains building geometry data (vertex, texture, ...)
 */
class Building3D : protected QOpenGLFunctions
{
public:
	/**
	 * @brief Building3D constructor for building test
	 * make a cubic building center on (0,0,0) and lenght 1
	 * Use cube.png as texture
	 */
	Building3D();

	/**
	 * @brief Building3D
	 * make a building with geometrics properties
	 *
	 * @warning currentry, param normal is not used in shader.
	 * In future version, it will be used
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param position  vertices 3D of points of faces
	 * @param normal    vertices 3D of normals
	 * @param textCord  vertices 2D of points in texture
	 * @param textPath  path of texture image
	 */
	Building3D(std::vector<QVector3D> position, std::vector<QVector3D> normal,
			std::vector<QVector2D> textCord, std::string textPath);
	virtual ~Building3D();

	/**
	 * @brief draw
	 * display building in OpenGLcityView
	 *
	 * @param shader  shader linked to QOpenGLWidget
	 */
	void draw(QOpenGLShaderProgram *shader);

private:

	/**
	 * @brief initGeometryObj
	 * construct attribute arrayBuf with all coordinates data
	 *
	 * @param position, normal, textCord  geometric data
	 */
	void initGeometryObj(std::vector<QVector3D> position, std::vector<QVector3D> normal,
			std::vector<QVector2D> textCord);

	/**
	 * @brief initTexture
	 * Initialize texture from path to bind it to shader
	 *
	 * @param textPath
	 */
	void initTexture(std::string textPath);

	int sizeArray = 0;
	QOpenGLBuffer arrayBuf;
	QOpenGLBuffer indexBuf;
	QOpenGLTexture *texture = nullptr;
};

#endif // BUILDING3D_H
