#pragma once
#include <glad/glad.h>

#include "Functionalities/VBO.hpp"

struct VAOattrib {
    GLuint layout; // attribute layout location in the shader. Is it the position, color, texture coords, etc.
    GLint numComponents; // how many numbers make up the attribute? (e.g., 2 for vec2, 3 for vec3, etc.)
    GLenum type = GL_FLOAT; // data type of each component, default GL_FLOAT
    GLsizei stride; // Suppose each vertex has 6 floats: 3 for position, 3 for color, then stride is 6 * sizeof(float)
    GLint offset = 0; // position(x,y,z) | color(r,g,b), the color starts after the 3rd float (after z), so it pretty much starts there.
};

class VAO
{
    GLuint ID;
    public:
    VAO()
    {
        glGenVertexArrays(1, &ID);
    }
    ~VAO()
    {
        glDeleteVertexArrays(1, &ID);
    }
    void Bind()
    {
        glBindVertexArray(ID);
    }
    void Unbind()
    {
        glBindVertexArray(0);
    }
    void LinkAttrib(VBO& vbo, GLuint layout, GLint numComponents, GLenum type, GLsizei stride, void* offset)
    {
        vbo.Bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride * sizeof(GLfloat), offset);
        glEnableVertexAttribArray(layout);
        vbo.Unbind();
    }
    void LinkAttrib(VBO& vbo, const VAOattrib& attrib)
    {
        vbo.Bind();
        glVertexAttribPointer(
            attrib.layout,
            attrib.numComponents,
            attrib.type,
            GL_FALSE,
            attrib.stride * sizeof(GLfloat),    // convert stride to bytes
            (void*)(attrib.offset * sizeof(GLfloat)) // convert offset to bytes
        );
        glEnableVertexAttribArray(attrib.layout);
        vbo.Unbind();
    }
};