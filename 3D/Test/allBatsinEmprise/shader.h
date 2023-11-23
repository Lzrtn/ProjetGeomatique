#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>


#include <string>
#include <unordered_map>
#include "glm/glm.hpp"
#include <GLFW/glfw3.h>

class Shader
{
public:
    Shader(std::string VertexShaderFilePath, std::string FragmentShaderFilePath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    unsigned int GetRendererID() {return m_RendererID;};

    //Set uniforms
    void setUniform1i(const std::string& name, int value);
    void setUniform1f(const std::string& name, float value);
    void setUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
    void setUniform3fv(const std::string& name, const glm::vec3 vector);
    void setUniformMat4f(const std::string& name, const glm::mat4 matrix);
private:
    GLuint LoadShaders(const std::string vertex_file_path,const std::string fragment_file_path);
    std::string m_VertexShaderFilePath;
    std::string m_FragmentShaderFilePath;
    unsigned int m_RendererID;
    unsigned int GetUniformLocation(const std::string &name);
    std::unordered_map<std::string, int> m_UniformLocationCache;
};

#endif // SHADER_H
