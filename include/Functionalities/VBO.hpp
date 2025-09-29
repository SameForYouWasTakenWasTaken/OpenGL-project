#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 color = {1.f, 1.f, 1.f}; // default white
};

class VBO
{
    GLuint ID;
public:
    template<typename T>
    VBO(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER,
                     data.size() * sizeof(T),
                     data.data(),
                     usage);
    }
    ~VBO()
    {
        glDeleteBuffers(1, &ID);
    }
    void Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
    void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};
