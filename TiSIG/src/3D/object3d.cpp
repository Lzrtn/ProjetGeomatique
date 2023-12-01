#include "object3d.h"

#include <glm/vec2.hpp>  // loadOBJ function use glm::vec2 & glm::vec2
#include <glm/vec3.hpp>

#include <cmath>

struct VertexData
{
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;
};

///////////////////////////////  READ OBJ FILES  ////////////////////////////////////
void loadOBJ(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals)
{
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	FILE * file = fopen(path, "r");
	if( file == NULL )
		throw std::runtime_error("fail to open file");

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 vertex;
			int matches = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			if (matches != 3)
				throw std::runtime_error("fail to read file (maybe a corrupted file)");
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 ){
			glm::vec2 uv;
			int matches = fscanf(file, "%f %f\n", &uv.x, &uv.y );
			if (matches != 2)
				throw std::runtime_error("fail to read file (maybe a corrupted file)");
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			glm::vec3 normal;
			int matches = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			if (matches != 3)
				throw std::runtime_error("fail to read file (maybe a corrupted file)");
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9)
				throw std::runtime_error("fail to read file (maybe a corrupted file)");
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}

	for( unsigned int i=0; i < vertexIndices.size(); i++ ){
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices.at(vertexIndex-1);
		out_vertices.push_back(vertex);
	}
	for( unsigned int i=0; i < uvIndices.size(); i++ ){
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs.at(uvIndex-1);
		out_uvs.push_back(uv);
	}
	for( unsigned int i=0; i < normalIndices.size(); i++ ){
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals.at(normalIndex-1);
		out_normals.push_back(normal);
	}
}

void readObj(const std::string & path, std::vector<QVector3D> & vertices, std::vector<QVector3D> & normals, std::vector<QVector2D> & uv) {
	// reset output
	vertices = {};
	normals = {};
	uv = {};

	// load file
	std::vector<glm::vec3> v, vn;
	std::vector<glm::vec2> vt;
	loadOBJ(path.c_str(), v, vt, vn);

	// convert from glm::vec* to QVector*D
	for (glm::vec3 vertice: v)
		vertices.push_back({vertice[0], vertice[1], vertice[2]});

	for (glm::vec2 texture: vt)
		uv.push_back({texture[0], texture[1]});

	for (glm::vec3 normal: vn)
		normals.push_back({normal[0], normal[1], normal[2]});

}

///////////////////////////////     OBJECT 3D    ////////////////////////////////////

Object3D::Object3D(const std::vector<QVector3D> &position, const std::vector<QVector3D> &normal,
		const std::vector<QVector2D> &textCoord, const std::string &textPath) :
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

Object3D::Object3D(const std::vector<QVector3D> &position, const std::vector<QVector2D> &textCoord, const std::string &textPath) :
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

Object3D::Object3D(const std::string &pathObj, const std::string & pathTexture) :
	indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuffer.create();
	this->indexBuffer.create();

	// read obj file
	std::vector<QVector3D> v, vn;
	std::vector<QVector2D> vt;
	readObj(pathObj, v, vn, vt);

	// Initializes geometry and texture
	this->initTexture(pathTexture);
	this->InitGeometryVectors(v, vn, vt);
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
