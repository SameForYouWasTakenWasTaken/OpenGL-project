#pragma once

#include <glad/glad.h>
#include <vector>

class EBO
{
    GLuint ID;
    public:
    EBO(const std::vector<GLuint>& indices, GLenum usage = GL_STATIC_DRAW)
    {
        Create(indices, usage);
    }
    EBO(const std::vector<GLuint>& indices, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices.data(), usage);
    }
    ~EBO()
    {
        glDeleteBuffers(1, &ID);
    }
    void Create(const std::vector<GLuint>& indices, GLenum usage)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), usage);
    }
    void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
    void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
};