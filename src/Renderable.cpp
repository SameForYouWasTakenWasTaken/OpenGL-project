#include "Renderable.hpp"

Renderable::Renderable(const std::vector<glm::vec3> Vertices,
                       const std::vector<GLuint> Indices,
                       const std::vector<VAOattrib> attrib)
: vertices(std::move(Vertices)), indices(std::move(Indices))
{
    vao = std::make_unique<VAO>();    
    vbo = std::make_unique<VBO>(vertices);
    ebo = std::make_unique<EBO>(indices);

    vao->Bind();
    vbo->Bind();
    ebo->Bind();

    for (const VAOattrib& at : attrib){
        vao->LinkAttrib(*vbo, at);
    }

    vao->Unbind();
    vbo->Unbind();
}

std::pair<bool, bool> Renderable::available_shader_sources()
{
    if (vertexSource.empty() || fragmentSource.empty())
    {
        spdlog::error("No vertex source or fragment source available! Vertex source :{}, Fragment source: {}", vertexSource.empty(), fragmentSource.empty());
    }

    return std::pair<bool, bool>(
        !vertexSource.empty(), !fragmentSource.empty() // Inverse, so if it is empty, it says false, if it isn't, it says true (it's there)
    );
}

void Renderable::set_shader_sources(const std::string& frag_src, const std::string& vert_src)
{
    fragmentSource = frag_src;
    vertexSource = vert_src;
}

void Renderable::update_shaders()
{
    auto shaders(available_shader_sources());

    if (shaders.first && shaders.second)
    {
        if (!shader)
            create_shaders();
        shader->Update(vertexSource.c_str(), fragmentSource.c_str());
    }
}

void Renderable::create_shaders()
{

    auto shaders = available_shader_sources();

    if (shaders.first && shaders.second)
    {
        if (!shader) {
            shader = std::make_unique<Shader>(vertexSource.c_str(), fragmentSource.c_str());
    }
    }

}

void Renderable::setPosition(glm::vec3& newPos)
{
    position = newPos;
}

void Renderable::Move(glm::vec3 pos)
{
    position += pos;
}

void Renderable::update_shaders(const std::string& frag_src, const std::string& vert_src) {
    set_shader_sources(frag_src, vert_src);
    create_shaders();
}

void Renderable::draw(GLenum usage = GL_TRIANGLES)
{
    if (!shader) {
        spdlog::error("Renderable shader not initialized!");
        return; // prevent segfault
    }
    shader->Use();
    vao->Bind();

    // Set uniforms. Position stuff haha
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    GLint modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


    // most important part of all !!!
    glDrawElements(usage, indices.size(), GL_UNSIGNED_INT, 0);
}