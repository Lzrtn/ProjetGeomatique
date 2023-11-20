#include "building3d.h"

#include <QVector2D>
#include <QVector3D>
//#include <iostream>

struct VertexData
{
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;
};

Building3D::Building3D() :
	indexBuf(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuf.create();
	this->indexBuf.create();

	// Initializes geometry and texture
	this->initTexture(":/cube.png");

	std::vector<QVector3D> p = {
		{ 2, 1,-1}, { 4, 1, 1}, { 4,-1,-1},
		{ 2, 1,-1}, { 4, 1, 1}, { 2,-1, 1},
		{ 4,-1,-1}, { 4, 1, 1}, { 2,-1, 1},
		{ 4,-1,-1}, { 2, 1,-1}, { 2,-1, 1}
	};
	std::vector<QVector3D> n = p;
	std::vector<QVector2D> t = {
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1},
		{-1,1}, {1, 1}, {1, -1}
	};
	this->initGeometryObj(p, n, t);
}

Building3D::Building3D(
		std::vector<QVector3D> position, std::vector<QVector3D> normal,
		std::vector<QVector2D> textCord, std::string textPath) :
	indexBuf(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuf.create();
	this->indexBuf.create();

	// Initializes geometry and texture
	this->initTexture(textPath);
	this->initGeometryObj(position, normal, textCord);
}

Building3D::~Building3D()
{
	this->arrayBuf.destroy();
	this->indexBuf.destroy();
}

void Building3D::initTexture(std::string textPath)
{
	this->texture = new QOpenGLTexture(QImage(textPath.c_str()).mirrored());
	this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
	this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
	this->texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Building3D::initGeometryObj(
		std::vector<QVector3D> position, std::vector<QVector3D> normal,
		std::vector<QVector2D> textCord)
{
	this->sizeArray = position.size();
	VertexData vertices[sizeArray];
	GLushort indices[sizeArray];
	for (int i=0; i<sizeArray; i++) {
		vertices[i] = {position[i], normal[i], textCord[i]};
		indices[i] = i;
	}

	// Transfer vertex data to VBO 0
	this->arrayBuf.bind();
	this->arrayBuf.allocate(vertices, this->sizeArray * sizeof(VertexData));

	// Transfer index data to VBO 1
	this->indexBuf.bind();
	this->indexBuf.allocate(indices, this->sizeArray * sizeof(GLushort));
}

void Building3D::draw(QOpenGLShaderProgram *program)
{
	// Tell OpenGL which VBOs to use
	this->arrayBuf.bind();
	this->indexBuf.bind();
	this->texture->bind();

	// Offset for position
	quintptr offset = 0;

	// Tell OpenGL programmable pipeline how to locate vertex position data
	int vertexLocation = program->attributeLocation("a_position");
	program->enableAttributeArray(vertexLocation);
	program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Offset for texture coordinate
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate normal position data
	int normalLocation = program->attributeLocation("a_normal");
	program->enableAttributeArray(normalLocation);
	program->setAttributeBuffer(normalLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

	// Offset for texture coordinate
	offset += sizeof(QVector3D);

	// Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
	int texcoordLocation = program->attributeLocation("a_texcoord");
	program->enableAttributeArray(texcoordLocation);
	program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

	// Draw cube geometry using indices from VBO 1
	this->glDrawElements(GL_TRIANGLE_STRIP, this->sizeArray, GL_UNSIGNED_SHORT, nullptr);
}
