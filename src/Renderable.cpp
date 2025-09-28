#include "Renderable.hpp"

void Renderable::CacheUniformLocations()
{
    modelLoc = glGetUniformLocation(shader->ID, "model");
    projLoc = glGetUniformLocation(shader->ID, "projection");
    viewLoc = glGetUniformLocation(shader->ID, "view");
}

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

void Renderable::SetIndices(const std::vector<GLuint>& Indices, GLenum usage)
{
    indices = Indices;

    ebo->Bind();
    ebo->Create(indices, usage);
    ebo->Unbind();

}

void Renderable::SetAspectRatio(unsigned int width, unsigned int height){
    
    float left = 0.0f;
    float right = static_cast<float>(width);
    float bottom = 0.0f;
    float top = static_cast<float>(height);
    float near = -1.0f;
    float far = 1.0f;

    projection = glm::ortho(0.0f, static_cast<float>(width),
                        0.0f, static_cast<float>(height),
                        near, far);

    // For 3d:
    // projection = glm::perspective(glm::radians(fov), width / (float)height, near, far);;
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
    bool GotUniformLocs = false;

    auto shaders(available_shader_sources());

    if (shaders.first && shaders.second)
    {
        if (!shader){
            create_shaders();
            GotUniformLocs = true;
        }
        shader->Update(vertexSource.c_str(), fragmentSource.c_str());
    }

    if (!GotUniformLocs)
        CacheUniformLocations();
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
    
    CacheUniformLocations();
}

void Renderable::update_shaders(const std::string& frag_src, const std::string& vert_src) {
    set_shader_sources(frag_src, vert_src);
    create_shaders();
}

void Renderable::setPosition(const glm::vec3& newPos)
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
    model = glm::mat4(1.f);
    model = glm::translate(model, position); // Set position

    // Rotate based on the rotation vec3
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1,0,0));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0,1,0));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0,0,1)); 

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
}

void Renderable::CommonDraw()
{
    if (!available_shader()) {
        spdlog::error("Renderable shader not initialized!");
        return; // prevent segfault
    }

    shader->Use();
    vao->Bind();

    UniformCalculations();

    BeforeDraw(); // Custom functionality if any other derived class wants to add its own cool twists :upside_down_face:
}