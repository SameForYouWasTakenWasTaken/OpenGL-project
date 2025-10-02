#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
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

    
    // transformation locations for CacheUniformLocations
    GLint modelLoc;

    void CacheUniformLocations();
    protected:
    
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    
    glm::vec3 position = {0,0,0};
    glm::vec3 rotation = {0,0,0}; // In degrees
    glm::vec3 scale    = {1,1,1}; // Default on all sides. 0,0,0 makes it practically.. Not there. 2,2,2 is double the size, for example

    glm::mat4 model = glm::mat4(1.f);
    std::vector<VAOattrib> attrib;
    
    void UniformCalculations(); // Keep it organized
    public:
    Renderable(std::vector<Vertex> Vertices,
        std::vector<GLuint> Indices = {0, 1, 2},
        std::vector<VAOattrib> attributes = {});
        virtual ~Renderable() = default;
        
        // Construction functions
        void LinkAttrib(VAOattrib& att);
        void SetIndices(const std::vector<GLuint>& Indices, GLenum usage = GL_STATIC_DRAW);
        void set_shader_sources(const std::string& frag_src, const std::string& vert_src); // Won't update the shaders.
        void create_shaders();
        void update_shaders();
        void update_shaders(const std::string& frag_src, const std::string& vert_src); // Update after setting the sources

        
        // Transforming, positions, etc
        void SetPosition(const glm::vec3& newPos);
        void Move(glm::vec3 pos);
        void Rotate(float degrees, ROTATION rotation_direction);
        void SetScale(const glm::vec3& amnt);
        void SetColor(const glm::vec3& col);
        // Information functions
        std::pair<bool, bool> available_shader_sources(); // first : vertex, second : fragment
        bool available_shader() {return shader->Available();}
        
        Shader* GetShader() { return shader.get(); }
        VAO* GetVAO() {return vao.get();}
        VBO* GetVBO() {return vbo.get();}
        EBO* GetEBO() {return ebo.get();}
        
        // Drawing, updates, rendering logic
        void CommonDraw(); // All the important stuff done when drawing
        
        virtual void draw(GLenum usage) 
        {
            CommonDraw(); // By default, only runs CommonDraw, but you can change it how you want.
            glDrawElements(usage, indices.size(), GL_UNSIGNED_INT, 0);
            // UNBINDING VAO IS HAPPENING IN THE RENDERER
        };

        virtual void BeforeDraw() = 0;
        virtual void update(float dt) = 0;
};