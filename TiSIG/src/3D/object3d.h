#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <vector>
#include <string>

/**
 * @brief
 * Object that contains building (or other 3D object) geometry data (vertex, texture, ...)
 */
class Object3D: protected QOpenGLFunctions
{
public:
	/**
	 * @brief
	 * make an object with geometrics properties
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
	Object3D(std::vector<QVector3D> position, std::vector<QVector3D> normal,
			std::vector<QVector2D> textCoord, std::string textPath);

	virtual ~Object3D();

	/**
	 * @brief draw
	 * display building in OpenGLcityView
	 *
	 * @param shader  shader linked to QOpenGLWidget
	 */
	void Draw(QOpenGLShaderProgram *shader);

private:

	/**
	 * @brief InitGeometryVectors
	 * construct attribute arrayBuf with all coordinates data
	 *
	 * @param position, normal, textCord  geometric data
	 */
	bool InitGeometryVectors(const std::vector<QVector3D> &position, const std::vector<QVector3D> &normal,
			const std::vector<QVector2D> &textCoord);

	/**
	 * @brief initTexture
	 * Initialize texture from path to bind it to shader
	 *
	 * @param textPath
	 */
	void initTexture(std::string textPath);

	int sizeArray = 0;
	QOpenGLBuffer arrayBuffer;
	QOpenGLBuffer indexBuffer;
	QOpenGLTexture *texture = nullptr;};

#endif // OBJECT3D_H