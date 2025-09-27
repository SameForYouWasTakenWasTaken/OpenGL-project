#pragma once
#include <glad/glad.h>
#include <vector>

class VBO
{
    GLuint ID;
public:
    VBO(const std::vector<glm::vec3>& vertices, GLenum usage = GL_STATIC_DRAW)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), usage);
    }
     VBO(const std::vector<glm::vec3>& vertices, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW)
    {
        glGenBuffers(1, &ID);
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), usage);
    }
    ~VBO()
    {
        glDeleteBuffers(1, &ID);
    }
    void Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
    void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};
