#include "uvbuffer.h"

#include "renderer.h"

unsigned int UVBuffer::getSize() const
{
    return size;
}

unsigned int UVBuffer::getCount() const
{
    return count;
}

UVBuffer::UVBuffer(const std::vector<glm::vec2> &data): size(data.size()*sizeof(glm::vec2))
{
    count = size/sizeof(data[0]);
    GLCall(glGenBuffers(1, &m_rendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER,size * sizeof(float), &data[0], GL_STATIC_DRAW));
}

UVBuffer::~UVBuffer() {
    GLCall(glDeleteBuffers(1,&m_rendererID));
}

void UVBuffer::Bind(int index) const{
    GLCall(glEnableVertexAttribArray(index));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_rendererID));
    GLCall(glVertexAttribPointer(
       index,                  // l'index du point à modifier.
       2,                  // taille d'un point
       GL_FLOAT,           // type de valeur
       GL_FALSE,           // normalisation?
       0,                  // taille des valeurs supplémentaires dans le vertexbuffer
       (void*)0            // offset : est-ce qu'on commence à un autre indice que 0
    ));

}

void UVBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
