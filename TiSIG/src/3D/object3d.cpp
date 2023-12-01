#include "object3d.h"

#include <cmath>

struct VertexData
{
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;
};

void readObj(const std::string & path, std::vector<QVector3D> & vertices, std::vector<QVector3D> & normals, std::vector<QVector2D> & uv) {

}

Object3D::Object3D(std::vector<QVector3D> position, std::vector<QVector3D> normal,
		std::vector<QVector2D> textCoord, std::string textPath) :
	indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuffer.create();
	this->indexBuffer.create();

	// Initializes geometry and texture
	this->initTexture(textPath);
	this->InitGeometryVectors(position, normal, textCoord);
}

Object3D::Object3D(std::vector<QVector3D> position, std::vector<QVector2D> textCoord, std::string textPath) :
	indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuffer.create();
	this->indexBuffer.create();

	// Initializes geometry and texture
	this->initTexture(textPath);
	std::vector<QVector3D> normal;
	int size = position.size();
	for (int i=0; i<size; i+=3) {
		// compute normal as vectorial product
		QVector3D norm = QVector3D::crossProduct(position[i+0], position[i+1]);
		normal.push_back(norm);
		normal.push_back(norm);
		normal.push_back(norm);
	}
	this->InitGeometryVectors(position, normal, textCoord);
}

Object3D::~Object3D()
{
	// free memory
	this->arrayBuffer.destroy();
	this->indexBuffer.destroy();
	if (this->texture != nullptr)
		delete this->texture;
}

void Object3D::initTexture(std::string textPath)
{
	// build the texture and wrap it
	this->texture = new QOpenGLTexture(QImage(textPath.c_str()).mirrored());
	this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
	this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
	this->texture->setWrapMode(QOpenGLTexture::Repeat);
}

bool Object3D::InitGeometryVectors(const std::vector<QVector3D> &position, const std::vector<QVector3D> &normal,
		const std::vector<QVector2D> &textCoord)
{
	// build geometrical coordinates with this format
	// arrayBuffer = {v1_x v1_y v1_z vn1_x vn1_y vn1_z vt1_x vt1_y // point 1
	//                v2_x ... vtn_y}
	// indexBuffer = {1 2 3 ... n} // list of points

	this->sizeArray = position.size();
	VertexData vertices[this->sizeArray];
	GLushort indices[this->sizeArray];
	for (int i=0; i<this->sizeArray; i++) {
		vertices[i] = {position[i], normal[i], textCoord[i]};
		indices[i] = i;
	}

	// Transfer vertex data to VBO 0
	this->arrayBuffer.bind();
	this->arrayBuffer.allocate(vertices, this->sizeArray * sizeof(VertexData));

	// Transfer index data to VBO 1
	this->indexBuffer.bind();
	this->indexBuffer.allocate(indices, this->sizeArray * sizeof(GLushort));

	return true;
}

void Object3D::Draw(QOpenGLShaderProgram *program)
{
	// Tell OpenGL which VBOs to use
	this->arrayBuffer.bind();
	this->indexBuffer.bind();
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
