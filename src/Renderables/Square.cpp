#include "Renderables/Square.hpp"

static std::vector<Vertex> ValidateCornerPoints(std::vector<Vertex>& corner_points)
{
    if (corner_points.size() < 4)
    {
        spdlog::error("Square doesn't have enough points! It needs 4 corner points.");
    }
    
    return corner_points;
}

Square::Square(std::vector<Vertex>& corner_points, std::vector<GLuint>& indices, std::string& pathToShadersource)
: Renderable(ValidateCornerPoints(corner_points), indices)
{

    auto ShaderSources = ParseShaderFile(pathToShadersource);

    VAOattrib pos;
    pos.layout = 0;
    pos.numComponents = 3;
    pos.stride = 6; // 3 vec3 (pos), 3 vec3 (color)
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
Square::Square(float a, float b, std::string& pathToShaderSource)

: Renderable(
    std::vector<Vertex>{
        {{-a/2,  b/2, 0.f}, {1.f, 1.f, 1.f}}, // Top left
        {{ a/2,  b/2, 0.f}, {1.f, 1.f, 1.f}}, // Top right
        {{ a/2, -b/2, 0.f}, {1.f, 1.f, 1.f}}, // Bottom right
        {{-a/2, -b/2, 0.f}, {1.f, 1.f, 1.f}}, // Bottom left
    },
    std::vector<GLuint>{
        0, 3, 2, // covers Bottom left triangle
        0, 1, 2  // covers Top right triangle
    }
)
{
    auto ShaderSources = ParseShaderFile(pathToShaderSource);

    VAOattrib pos;
    pos.layout = 0;
    pos.numComponents = 3;
    pos.stride = 6; // 3 vec3 (pos), 3 vec3 (color)
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

Square::Square(float a, std::string& pathToShaderSource)
: Square(a, a, pathToShaderSource) // Calls the rectangle constructor with equal sides
{}

void Square::draw(GLenum usage)
{
    //SetIndices(indices);
    CommonDraw();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}