#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <iostream>
#include <signal.h>

#include "vertexarray.h"
#include "shader.h"
#include "object.h"

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__));

void GLClearError();

bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
public:
    Renderer();
    void Draw(const VertexArray& va, const Object& o, const Shader& shader) const;
    void Clear() const;
};

#endif // RENDERER_H
