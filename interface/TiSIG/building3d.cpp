#include "building3d.h"

#include <QVector2D>
#include <QVector3D>

struct VertexData
{
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;
};

Building3D::Building3D(std::vector<QVector3D> position, std::vector<QVector3D> normal,
		std::vector<QVector2D> textCoord, std::string textPath) :
	indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuffer.create();
	this->indexBuffer.create();

	// Initializes geometry and texture
	this->initTexture(textPath);
	this->InitGeometryObj(position, normal, textCoord);
}

Building3D::~Building3D()
{
	// free memory
	this->arrayBuffer.destroy();
	this->indexBuffer.destroy();
	if (this->texture != nullptr)
		delete this->texture;
}

void Building3D::initTexture(std::string textPath)
{
	// build the texture and wrap it
	this->texture = new QOpenGLTexture(QImage(textPath.c_str()).mirrored());
	this->texture->setMinificationFilter(QOpenGLTexture::Nearest);
	this->texture->setMagnificationFilter(QOpenGLTexture::Linear);
	this->texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Building3D::InitGeometryObj(const std::vector<QVector3D> &position, const std::vector<QVector3D> &normal,
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
}

void Building3D::Draw(QOpenGLShaderProgram *program)
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


/////////////////////////////  Building 3D Factory  //////////////////////////////////////

Building3DFactory::Building3DFactory(const std::vector<QVector3D> &position,
									 const std::vector<QVector3D> &normal,
									 const std::vector<QVector2D> &textCoord,
									 const std::string &textPath) :
	position(position), normal(normal), textCoord(textCoord), textPath(textPath)
{}

Building3DFactory::Building3DFactory(const int version)
{
	/*
	 * version 0 and version 1 are same with different origin
	 * regular tetrahedre
	 */
	switch (version)
	{
	case 0:
		position = {
			{ 2, 1,-1}, { 4, 1, 1}, { 4,-1,-1},
			{ 2, 1,-1}, { 4, 1, 1}, { 2,-1, 1},
			{ 4,-1,-1}, { 4, 1, 1}, { 2,-1, 1},
			{ 4,-1,-1}, { 2, 1,-1}, { 2,-1, 1}
		};
		normal = position; // wrong values, but without light, it has no effects
		textCoord = {
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1}
		};
		textPath = ":/cube.png";
		break;

	case 1:
		position = {
			{ -4, 1,-1}, { -2, 1, 1}, { -2,-1,-1},
			{ -4, 1,-1}, { -2, 1, 1}, { -4,-1, 1},
			{ -2,-1,-1}, { -2, 1, 1}, { -4,-1, 1},
			{ -2,-1,-1}, { -4, 1,-1}, { -4,-1, 1}
		};
		normal = position; // wrong values, but without light, it has no effects
		textCoord = {
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1},
			{-1,1}, {1, 1}, {1, -1}
		};
		textPath = ":/cube.png";
		break;
	case 2:
		position = {
			{-2, 18, 0}, {-2, 22, 0}, {2, 22, 0},
			{2, 22, 0}, {2, 18, 0}, {-2, 18, 0}
		};
		normal = position;
		textCoord = {
			{0, 0}, {1, 0}, {1, 1},
			{0, 0}, {0, 1}, {1, 1}
		};
		textPath = ":/cube.png";
		break;
	case 3:
	default:
		position = {
			{-2, -2, 0}, {-2,  2, 0}, { 2,  2, 0},
			{-2, -2, 0}, { 2, -2, 0}, { 2,  2, 0},
		};
		normal = position;
		textCoord = {
			{0, 0}, {0, 1}, {1, 1},
			{0, 0}, {1, 0}, {1, 1},
		};
		textPath = ":/rose des vents.png";
		break;
	}

}

Building3D * Building3DFactory::NewBuilding() const
{
	return new Building3D(position, normal, textCoord, textPath);
}
