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

class Renderable {
private:
    std::string fragmentSource;
    std::string vertexSource;

    std::unique_ptr<VAO> vao;
    std::unique_ptr<VBO> vbo;
    std::unique_ptr<EBO> ebo;
    std::unique_ptr<Shader> shader;
    
protected:
    std::vector<glm::vec3> vertices;
    std::vector<GLuint> indices;
    glm::vec3 position = {0,0,0};
public:
    Renderable(std::vector<glm::vec3> Vertices,
           std::vector<GLuint> Indices = {},
           std::vector<VAOattrib> attrib = {});


    void set_shader_sources(const std::string& frag_src, const std::string& vert_src); // Won't update the shaders.
    void create_shaders();
    void update_shaders();
    void update_shaders(const std::string& frag_src, const std::string& vert_src); // Update after setting the sources
   
    virtual void setPosition(glm::vec3& newPos);
    virtual void Move(glm::vec3 pos);
    virtual std::pair<bool, bool> available_shader_sources(); // first : vertex, second : fragment

    void draw(GLenum usage);
    void update(float dt);
};