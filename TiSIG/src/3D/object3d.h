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
	 * @throw exception if vertices have different sizes
	 *
	 * @param position  vertices 3D of points of faces
	 * @param normal    vertices 3D of normals
	 * @param textCord  vertices 2D of points in texture
	 * @param textPath  path of texture image
	 */
	Object3D(const std::vector<QVector3D> & position, const std::vector<QVector3D> & normal,
			const std::vector<QVector2D> & textCoord, const std::string & textPath);

	/**
	 * @brief
	 * make an object with geometrics properties. Auto make normals as vector product of coords
	 *
	 * @warning normals compute needs that faces are direct oriented
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param position  vertices 3D of points of faces
	 * @param textCord  vertices 2D of points in texture
	 * @param textPath  path of texture image
	 */
	Object3D(const std::vector<QVector3D> & position,
			const std::vector<QVector2D> & textCoord, const std::string & textPath);

	/**
	 * @brief
	 * make an object with geometrics properties from .obj file
	 *
	 * @warning normals compute needs that faces are direct oriented
	 *
	 * @throw exception if files are unvalid
	 *
	 * @param pathObj:		path to the .obj file
	 * @param pathTexture:	path to the texture file
	 */
	Object3D(const std::string & pathObj, const std::string &pathTexture);

	virtual ~Object3D() = default;

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
	QOpenGLTexture *texture = nullptr;

};


class Object3DFactory {
public:
	virtual ~Object3DFactory() = default;
	virtual Object3D* New() const = 0;
};

#endif // OBJECT3D_H
