#include "Renderables/Circle.hpp"

#define PI 3.14159265358979323846 

std::vector<Vertex> Create(float radius, const glm::vec3& position, size_t iter_num)
{
    std::vector<Vertex> _vertices;

    _vertices.push_back({{position.x, position.y, 0}, {1.f, 1.f, 1.f}}); // center
    
    for (size_t i = 0; i <= iter_num; i++) {  // <= to close the loop
        float theta = (2 * PI * i) / iter_num;
        float x = radius * cos(theta) + position.x;
        float y = radius * sin(theta) + position.y;
        _vertices.push_back({{x, y, 0}, {5.f, 5.f, 5.f}});
    }

    return _vertices;
}

Circle::Circle(float r, size_t iter_num, std::string& pathToShaderFile)
: radius(r), Renderable(Create(r, position, iter_num))
{
    auto ShaderSources = ParseShaderFile(pathToShaderFile);

    VAOattrib pos;
    pos.layout = 0; // Default
    pos.numComponents = 3;
    pos.stride = 6;
    pos.offset = 0;

    VAOattrib color;
    color.layout = 1; // Default
    color.numComponents = 3;
    color.stride = 6;
    color.offset = 3;

    LinkAttrib(pos);
    LinkAttrib(color);

    set_shader_sources(ShaderSources.FragmentSource, ShaderSources.VertexSource);
    create_shaders();
}


void Circle::draw(GLenum usage) {
    CommonDraw();
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}