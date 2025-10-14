#include "Renderables/Triangle.hpp"

Triangle::Triangle(std::vector<Vertex>& vertices, std::string& pathToShaderSource)
: Renderable(vertices)
{
    auto ShaderSources = ParseShaderFile(pathToShaderSource);

	VAOattrib pos;
	pos.layout = 0;
	pos.numComponents = 3;
	pos.stride = Vertex::stride();
	pos.offset = offsetof(Vertex, position);

	VAOattrib color = pos;
	color.layout = 1;
	color.numComponents = 4;
	color.offset = offsetof(Vertex, color);

    LinkAttrib(pos);
    LinkAttrib(color);

    set_shader_sources(ShaderSources.FragmentSource, ShaderSources.VertexSource);
    create_shaders();
}

void Triangle::BeforeDraw()
{return;}