#include "Renderables/Circle.hpp"
#include "Renderables/Triangle.hpp"

#define PI 3.14159265358979323846 

std::vector<Vertex> Create(float radius, const glm::vec3& position, size_t iter_num)
{
    std::vector<Vertex> _vertices;

    _vertices.push_back({{position.x, position.y, 0}, {1.f, 1.f, 1.f, 1.f}}); // center
    
    for (size_t i = 0; i <= iter_num; i++) {  // <= to close the loop
        float theta = (2 * PI * i) / iter_num;
        float x = radius * cos(theta) + position.x;
        float y = radius * sin(theta) + position.y;
        _vertices.push_back({{x, y, 0}, {0.f, 0.f, .5f, 1.f}});
    }

    return _vertices;
}

Circle::Circle(float r, size_t iter_num, std::string& pathToShaderFile)
: radius(r), Renderable(Create(r, {0,0,0}, iter_num))
{
    auto ShaderSources = ParseShaderFile(pathToShaderFile);

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


void Circle::draw(GLenum usage) {
    CommonDraw();
    glDrawArrays(GL_TRIANGLE_FAN, 0, vertices.size());
}