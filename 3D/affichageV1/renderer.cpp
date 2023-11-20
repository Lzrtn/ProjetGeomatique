#include "renderer.h"

Renderer::Renderer()
{

}

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line){
    while (GLenum error = glGetError()){
        std::cout << "[OpenGL error] " <<error<<" : "<<function<<" "<<file<<":"<<line<<std::endl;
        return false;
    }
    return true;
}


void Renderer::Draw(const VertexArray &va, const Object &o, const Shader &shader) const
{
    shader.Bind();
    va.Bind();
    o.Bind();
    o.Draw();
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
