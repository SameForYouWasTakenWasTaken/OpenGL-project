#include "Renderable.hpp"

Renderable::Renderable(const std::vector<Vertex> Vertices,
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

void Renderable::LinkAttrib(VAOattrib& att)
{
    vao->Bind();
    vao->LinkAttrib(*vbo, att);
    vao->Unbind();
}

void Renderable::SetIndices(std::vector<GLuint>& Indices, GLenum usage)
{
    indices = Indices;

    ebo->Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
                indices.size() * sizeof(GLuint), 
                indices.data(), 
                usage); // or GL_DYNAMIC_DRAW if you'll update often
    ebo->Unbind();

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

void Renderable::update_shaders(const std::string& frag_src, const std::string& vert_src) {
    set_shader_sources(frag_src, vert_src);
    create_shaders();
}

void Renderable::setPosition(glm::vec3& newPos)
{
    position = newPos;
}

void Renderable::Move(glm::vec3 pos)
{
    position += pos;
}

void Renderable::Rotate(float degrees, ROTATION dir)
{
    switch(dir)
    {
        case ROTATION::NORTH: rotation.y += degrees; break;
        case ROTATION::SOUTH: rotation.y -= degrees; break;
        case ROTATION::EAST:  rotation.x += degrees; break;
        case ROTATION::WEST:  rotation.x -= degrees; break;
    }
    
    // Optional: normalize angles to [0, 360)
    rotation.x = fmod(rotation.x, 360.0f);
    rotation.y = fmod(rotation.y, 360.0f);
    rotation.z = fmod(rotation.z, 360.0f);
}

void Renderable::UniformCalculations()
{
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, position); // Set position

    // Rotate based on the rotation vec3
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0,0,1));
    
    
    GLint modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}
void Renderable::draw(GLenum usage = GL_TRIANGLES)
{
    if (!shader) {
        spdlog::error("Renderable shader not initialized!");
        return; // prevent segfault
    }

    shader->Use();
    vao->Bind();

    BeforeDraw(); // Custom functionality if any other derived class wants to add its own cool twists :upside_down_face:
    UniformCalculations();

    // most important part of all !!!
    glDrawElements(usage, indices.size(), GL_UNSIGNED_INT, 0);
    
}