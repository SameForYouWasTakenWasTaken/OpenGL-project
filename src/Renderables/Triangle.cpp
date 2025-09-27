#include "Renderables/Triangle.hpp"

Triangle::Triangle(std::vector<Vertex> vertices, std::string& pathToShaderSource)
: Renderable(vertices)
{
    auto ShaderSources = ParseShaderFile(pathToShaderSource);

    VAOattrib pos;
    pos.layout = 0;
    pos.numComponents = 3;
    pos.stride = 6;
    pos.offset = 0;

    VAOattrib color;
    color.layout = 1;
    color.numComponents = 3;
    color.stride = 6;
    color.offset = 3;

    LinkAttrib(pos);
    LinkAttrib(color);

    set_shader_sources(ShaderSources.FragmentSource, ShaderSources.VertexSource);
    create_shaders();
}

void Triangle::BeforeDraw()
{return;}