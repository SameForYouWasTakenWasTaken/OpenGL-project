#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // for glm::translate, glm::rotate, glm::scale
#include <glm/gtc/type_ptr.hpp>         // for glm::value_ptr


#include "Functionalities/EBO.hpp"
#include "Functionalities/VAO.hpp"
#include "Functionalities/VBO.hpp"

#include "Functionalities/Shader.hpp"

enum class ROTATION
{
    NORTH,
    WEST,
    EAST,
    SOUTH
};

class Renderable {
private:
    std::string fragmentSource;
    std::string vertexSource;

    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    std::unique_ptr<Shader> shader;
    
    void UniformCalculations(); // Keep it organized
protected:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    glm::vec3 position = {0,0,0};
    glm::vec3 rotation = {0,0,0};
public:
    Renderable(std::vector<Vertex> Vertices,
        std::vector<GLuint> Indices = {0, 1, 2},
        std::vector<VAOattrib> attrib = {});
        
        
    void LinkAttrib(VAOattrib& att);
    void SetIndices(std::vector<GLuint>& Indices, GLenum usage = GL_STATIC_DRAW);
    void set_shader_sources(const std::string& frag_src, const std::string& vert_src); // Won't update the shaders.
    void create_shaders();
    void update_shaders();
    void update_shaders(const std::string& frag_src, const std::string& vert_src); // Update after setting the sources
   
    void setPosition(glm::vec3& newPos);
    void Move(glm::vec3 pos);
    void Rotate(float degrees, ROTATION rotation_direction);

    std::pair<bool, bool> available_shader_sources(); // first : vertex, second : fragment

    void draw(GLenum usage);
    virtual void BeforeDraw() = 0;
    virtual void update(float dt) = 0;
};