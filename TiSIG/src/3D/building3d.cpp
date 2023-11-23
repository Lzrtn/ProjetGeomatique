#include "building3d.h"

#include <QVector2D>
#include <QVector3D>

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>


#include <iostream>

#include <stdlib.h>
#include <stdio.h>

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
	this->InitGeometryVectors(position, normal, textCoord);
}

Building3D::Building3D(const std::string &objPath, const std::string &textPath) :
	indexBuffer(QOpenGLBuffer::IndexBuffer)
{
	this->initializeOpenGLFunctions();

	// Generate 2 VBOs
	this->arrayBuffer.create();
	this->indexBuffer.create();

	// Initializes geometry and texture
	this->initTexture(textPath);
	this->InitGeometryObj(objPath);
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

bool Building3D::InitGeometryVectors(const std::vector<QVector3D> &position, const std::vector<QVector3D> &normal,
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
		std::cout << position[i].x() << " " << position[i].y() << " " << position[i].z()
				  << "\t" << normal[i].x() << " " << normal[i].y() << " " << normal[i].z()
				  << "\t" << textCoord[i].x() << " " << textCoord[i].y() << "\n";
		indices[i] = i;
	}
	std::cout << std::endl;

	// Transfer vertex data to VBO 0
	this->arrayBuffer.bind();
	this->arrayBuffer.allocate(vertices, this->sizeArray * sizeof(VertexData));

	// Transfer index data to VBO 1
	this->indexBuffer.bind();
	this->indexBuffer.allocate(indices, this->sizeArray * sizeof(GLushort));

	return true;
}

bool Building3D::InitGeometryObj(const std::string &objPath)
{
	/*
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< QVector3D > temp_vertices, final_vertices;
	std::vector< QVector2D > temp_uvs, final_uv;
	std::vector< QVector3D > temp_normals, final_normals;

	FILE * file = fopen(objPath.c_str(), "r");
	if( file == NULL ){
		printf("Impossible to open the file !\n");
		return false;
	}

	while( 1 ){

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		// else : parse lineHeader
		if ( strcmp( lineHeader, "v" ) == 0 ){
			glm::vec3 v;
			fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z );
			std::cout << "v " << v.x << " " << v.y << " " << v.z << "\n";
			temp_vertices.push_back({v.x, v.y, v.z});
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 ){
			float x, y;
			glm::vec2 v;
			fscanf(file, "%f %f\n", &v.x, &v.y );
			std::cout << "vt " << v.x << " " << v.y << " " << "\n";
			temp_uvs.push_back({v.x, v.y});
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 ){
			float x, y, z;
			fscanf(file, "%f %f %f\n", &x, &y, &z );
			std::cout << "vn " << x << " " << y << " " << z << "\n";
			temp_normals.push_back({x, y, z});
		}
		else if ( strcmp( lineHeader, "f" ) == 0 ){
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
				return false;
			}
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
		QVector3D vertex = temp_vertices[ vertexIndex-1 ];
		final_vertices.push_back(vertex);
	}
	for( unsigned int i=0; i < uvIndices.size(); i++ ){
		unsigned int uvIndex = uvIndices[i];
		QVector2D uv = temp_uvs[ uvIndex-1 ];
		final_uv.push_back(uv);
	}
	for( unsigned int i=0; i < normalIndices.size(); i++ ){
		unsigned int normalIndex = normalIndices[i];
		QVector3D normal = temp_normals[ normalIndex-1 ];
		final_normals.push_back(normal);
	}

	std::cout << final_normals.size() << std::endl;

	return this->InitGeometryVectors(final_vertices, final_normals, final_uv);
	*/

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		FILE * file = fopen(objPath.c_str(), "r");
		if( file == NULL ){
			printf("Impossible to open the file !\n");
			return false;
		}

		while( 1 ){

			char lineHeader[128];
			// read the first word of the line
			int res = fscanf(file, "%s", lineHeader);
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop.
			std::cout << lineHeader << "\n";

			if ( strcmp( lineHeader, "v" ) == 0 ){
				char l[128];
				fscanf(file, "%s", l);
				float f = strtof(&l[0], NULL);
				std::cout << "value : " << l << "=" << f << " ";
				fscanf(file, "%s", l);
				std::cout << strtof(l, NULL) << " ";
				fscanf(file, "%s", l);
				std::cout << strtof(l, NULL) << "\n";
			}

			continue;

			// else : parse lineHeader
			if ( strcmp( lineHeader, "v" ) == 0 ){
				glm::vec3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				std::cout << "v " << vertex.x << " " << vertex.y << " " << vertex.z << std::endl;
				temp_vertices.push_back(vertex);
			}
			else if ( strcmp( lineHeader, "vt" ) == 0 ){
				glm::vec2 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y );
				temp_uvs.push_back(uv);
			}
			else if ( strcmp( lineHeader, "vn" ) == 0 ){
				glm::vec3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
				temp_normals.push_back(normal);
			}
			else if ( strcmp( lineHeader, "f" ) == 0 ){
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
				if (matches != 9){
					printf("File can't be read by our simple parser : ( Try exporting with other options\n");
					return false;
				}
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
			glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
			//out_vertices.push_back(vertex);
		}
		for( unsigned int i=0; i < uvIndices.size(); i++ ){
			unsigned int uvIndex = uvIndices[i];
			glm::vec2 uv = temp_uvs[ uvIndex-1 ];
			//out_uvs.push_back(uv);
		}
		for( unsigned int i=0; i < normalIndices.size(); i++ ){
			unsigned int normalIndex = normalIndices[i];
			glm::vec3 normal = temp_normals[ normalIndex-1 ];
			//out_normals.push_back(normal);
		}

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
	position(position), normal(normal), textCoord(textCoord), textPath(textPath), modeObj(false)
{}

Building3DFactory::Building3DFactory(const std::string &objPath, const std::string &textPath):
	textPath(textPath), objPath(objPath), modeObj(true)
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
		modeObj = false;
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
		modeObj = false;
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
		modeObj = false;
		break;
	case 3:
		position = {
			{-2, -2, 5}, {-2,  2, 5}, { 2,  2, 5},
			{-2, -2, 5}, { 2, -2, 5}, { 2,  2, 5},
		};
		normal = position;
		textCoord = {
			{0, 0}, {0, 1}, {1, 1},
			{0, 0}, {1, 0}, {1, 1},
		};
		textPath = ":/rose des vents.png";
		modeObj = false;
		break;
	case 4:
	default:
		objPath = "/home/formation/Bureau/miniPrjSIG/TSI23_minisig/TiSIG/src/3D/cube.obj";
		textPath = ":/cube.png";
		modeObj = true;
	}

}

Building3D * Building3DFactory::NewBuilding() const
{
	if (this->modeObj)
		return new Building3D(this->objPath, this->textPath);
	else
		return new Building3D(this->position, this->normal, this->textCoord, this->textPath);
}
