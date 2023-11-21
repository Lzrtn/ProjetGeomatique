#include "object.h"

#include <iostream>
#include "renderer.h"
#include "glm/gtx/transform.hpp"

#include <exception>
#include <cstring>



Object::Object(std::vector<glm::vec3> vertices, std::vector<glm::vec2> uvs, std::string texturePath):m_vb(0), m_uvsb(0), m_texture(0), position(0,0,0), rotationAngles(0,0,0)
{
     m_vb = new VertexBuffer(vertices);
     m_uvsb = new UVBuffer(uvs);

     m_texture = new Texture(texturePath);
}


Object::Object(const char *pathObj, std::string texturePath){


    std::vector<glm::vec3> out_vertices;
    std::vector<glm::vec2> out_uvs;
    std::vector<glm::vec3> out_normals;
    loadOBJ(pathObj, out_vertices, out_uvs, out_normals);

    m_vb = new VertexBuffer(out_vertices);
    m_uvsb = new UVBuffer(out_uvs);
    m_texture = new Texture(texturePath);
    m_normales = new VertexBuffer(out_normals);
}

//Object::~Object(){}

bool Object::loadOBJ(const char *path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs,
                     std::vector<glm::vec3> &out_normals)
{
    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
    std::vector< glm::vec3 > temp_vertices;
    std::vector< glm::vec2 > temp_uvs;
    std::vector< glm::vec3 > temp_normals;

    FILE * file = fopen(path, "r");
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
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
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
        out_vertices.push_back(vertex);
    }
    for( unsigned int i=0; i < uvIndices.size(); i++ ){
        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[ uvIndex-1 ];
        out_uvs.push_back(uv);
    }
    for( unsigned int i=0; i < normalIndices.size(); i++ ){
        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];
        out_normals.push_back(normal);
    }

    return true;
}




Object::~Object()
{
    delete m_vb;
    if (m_uvsb) delete m_uvsb;
    if (m_texture) delete m_texture;
}

void Object::Bind() const
{
    m_vb->Bind(0);
    if (m_uvsb) m_uvsb->Bind(1);
    if (m_texture) m_texture->Bind(0);
}

void Object::Unbind() const
{
    m_vb->Unbind();
    if (m_uvsb) m_uvsb->Unbind();
    if (m_texture) m_texture->Unbind();
}



void Object::Draw() const
{
    GLCall(glDrawArrays(GL_TRIANGLES,0, m_vb->getSize()));
}

glm::mat4 Object::getModelMatrix()
{
    glm::mat4 m = glm::rotate(glm::translate(glm::mat4(1), position), rotationAngles.x, glm::vec3(1,0,0));
    m=glm::rotate(m, rotationAngles.y, glm::vec3(0,1,0));
    m=glm::rotate(m, rotationAngles.z, glm::vec3(0,0,1));
    return m;
}
