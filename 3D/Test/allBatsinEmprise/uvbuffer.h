#ifndef UVBUFFER_H
#define UVBUFFER_H

#include <vector>
#include <glm/glm.hpp>


class UVBuffer
{
private:
    unsigned int m_rendererID;
    unsigned int size;
    unsigned int count;
public:
    UVBuffer(const std::vector<glm::vec2> &data);
    virtual ~UVBuffer();

    void Bind(int index) const;
    void Unbind() const;
    unsigned int getSize() const;
    unsigned int getCount() const;
};

#endif // UVBUFFER_H
