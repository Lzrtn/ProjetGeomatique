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
	 * @brief
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
			std::vector<QVector2D> textCoord, std::string textPath);

	/**
	 * @brief
	 * make a building with geometrics properties
	 *
	 * @warning currentry, param normal is not used in shader.
	 * In future version, it will be used
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param objPath:	vertices 3D of points of faces
	 * @param normal:	vertices 3D of normals
	 * @param textCord:	vertices 2D of points in texture
	 * @param textPath:	path of texture image
	 */
	Building3D(const std::string &objPath, const std::string &textPath);
	virtual ~Building3D();

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
	 * @brief initGeometryObj
	 * construct attribute arrayBuf with all coordinates data
	 *
	 * @param objPath
	 */
	bool InitGeometryObj(const std::string &objPath);

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

class Building3DFactory
{
public:
	/**
	 * @brief Building3DFactory produce buildings from this geometry
	 *
	 * All faces may have 3 points;
	 *
	 * @param position: 3D coordinate of points
	 * @param normal: 3D vector normal to faces. Currently unused, but may
	 *   be used in a futur version with light effects
	 * @param textCoord: 2D coordinate of point in texture image
	 * @param textPath: path of texture image. Only one image can be used
	 */
	Building3DFactory(
			const std::vector<QVector3D> &position,
			const std::vector<QVector3D> &normal,
			const std::vector<QVector2D> &textCoord,
			const std::string &textPath);

	/**
	 * @brief Building3DFactory produce buildings from this geometry
	 *
	 * All faces may have 3 points;
	 *
	 * @param objPath: path to .obj file
	 * @param textPath: path of texture image. Only one image can be used
	 */
	Building3DFactory(
			const std::string &objPath,
			const std::string &textPath);

	/**
	 * @brief Building3DFactory produce buildings for test
	 *
	 * @param version: version of building (between 0 and 1 included)
	 */
	Building3DFactory(const int version = 0);

	/**
	 * @brief Genere a building
	 *
	 * @Warning: risk of memory lost. equivalent to `new Building(...)`
	 * Remember to delete building after use
	 */
	Building3D * NewBuilding() const;

private:
	std::vector<QVector3D> position;
	std::vector<QVector3D> normal;
	std::vector<QVector2D> textCoord;
	std::string textPath;
	std::string objPath;
	bool modeObj;
};

#endif // BUILDING3D_H
