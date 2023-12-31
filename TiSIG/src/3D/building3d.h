﻿#ifndef BUILDING3D_H
#define BUILDING3D_H

#include "object3d.h"

/**
 * @brief The Building3D class
 * Object that contains building geometry data (vertex, texture, ...), used in
 * OpenGLcityView. Do not use directly the constructor
 *
 * @see Building3DFactory, Object3DStorage and Layer3D to add objects
 */
class Building3D : public Object3D
{
public:

	/**
	 * @brief
	 * make a building with geometrics properties
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param vertices :	vertices 3D of points of faces
	 * @param normals :		vertices 3D of normals
	 * @param uv :			vertices 2D of points in texture
	 * @param pathTexture :	path of texture image
	 */
	Building3D(const std::vector<QVector3D> & vertices, const std::vector<QVector3D> & normals,
			   const std::vector<QVector2D> & uv, const std::string & pathTexture,
			   const QVector3D & symbo) :
		Object3D(vertices, normals, uv, pathTexture, symbo)
	{};

	/**
	 * @brief
	 * make a building with geometrics properties and compute automaticly normals

	 * @warning need direct-oriented faces to compute normals
	 *
	 * @throw exception if vertices have different sizes
	 *
	 * @param vertices :	vertices 3D of points of faces
	 * @param uv :			vertices 2D of points in texture
	 * @param pathTexture :	path of texture image
	 */
	Building3D(const std::vector<QVector3D> & position,
			   const std::vector<QVector2D> & textCoord, const std::string & textPath,
			   const QVector3D & symbo) :
		Object3D(position, textCoord, textPath, symbo)
	{};
};

class Building3DFactory : public Object3DFactory
{
public:
	/**
	 * @brief Building3DFactory produce buildings from this geometry
	 *
	 * All faces may have 3 points: give only trianglulated vertices with same size
	 *
	 * @param vertices :	3D coordinate of points
	 * @param normals :		3D vector normal to faces
	 * @param uv :			2D coordinate of point in texture image
	 * @param pathTexture :	path of texture image. Only one image can be used
	 */
	Building3DFactory(
			const std::vector<QVector3D> &vertices,
			const std::vector<QVector3D> &normals,
			const std::vector<QVector2D> &uv,
			const std::string &pathTexture) :
		vertices(vertices), normals(normals), uv(uv), pathTexture(pathTexture), symbo({0.5, .5, .5})
	{};

	/**
	 * @brief Building3DFactory produce buildings from this geometry
	 *
	 * All faces may have 3 points: give only trianglulated vertices with same size
	 *
	 * @param vertices :	3D coordinate of points
	 * @param normals :		3D vector normal to faces
	 * @param uv :			2D coordinate of point in texture image
	 * @param pathTexture :	path of texture image. Only one image can be used
	 * @param symbo:		color of building (default gray 50%)
	 */
	Building3DFactory(
			const std::vector<QVector3D> &vertices,
			const std::vector<QVector3D> &normals,
			const std::vector<QVector2D> &uv,
			const std::string &pathTexture,
			const QVector3D &symbo) :
		vertices(vertices), normals(normals), uv(uv), pathTexture(pathTexture), symbo(symbo)
	{};

	/**
	 * @brief Genere a building
	 *
	 * @warning risk of memory lost. equivalent to `new Building(...)`: don't forget
	 *	to delete building
	 */
	Building3D * New() const;

private:
	std::vector<QVector3D> vertices;
	std::vector<QVector3D> normals;
	std::vector<QVector2D> uv;
	std::string pathTexture;
	QVector3D symbo;
};

#endif // BUILDING3D_H
