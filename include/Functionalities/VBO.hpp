#pragma once
#include <glad/glad.h>
#include <vector>
#include <glm/vec3.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec4 color = {1.f, 1.f, 1.f, 1.f}; // default white

	static constexpr GLsizei stride() noexcept {
		return static_cast<GLsizei>(sizeof(Vertex));
	}
};

class VBO
{
    GLuint ID;
public:
    template<typename T>
    VBO(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW)
    {
        Create(data, usage);
    }
    void Create(const std::vector<Vertex>& vertices, GLenum usage = GL_STATIC_DRAW)
    {
        glDeleteBuffers(1, &ID); // Delete
        glGenBuffers(1, &ID); // Recreate
        glBindBuffer(GL_ARRAY_BUFFER, ID);
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(Vertex),
                     vertices.data(),
                     usage);
    }
    ~VBO()
    {
        glDeleteBuffers(1, &ID);
    }
    void Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
    void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
};
