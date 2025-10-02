#include "Renderables/Square.hpp"

static std::vector<Vertex> ValidateCornerPoints(std::vector<Vertex>& corner_points)
{
    if (corner_points.size() < 4)
    {
        spdlog::error("Square doesn't have enough points! It needs 4 corner points.");
    }
    
    return corner_points;
}

Square::Square(float a, float b, float c, std::string& pathToShaderSource) 
: Renderable(
    std::vector<Vertex>{
        {{-a/2,  b/2, 0.f}, {1.f, 1.f, 1.f}}, // Top left
        {{ a/2,  b/2, 0.f}, {1.f, 1.f, 1.f}}, // Top right
        {{ a/2, -b/2, 0.f}, {1.f, 1.f, 1.f}}, // Bottom right
        {{-a/2, -b/2, 0.f}, {1.f, 1.f, 1.f}}, // Bottom left

        {{-a/2,  b/2, c}, {1.f, 0.f, 1.f}}, // 4 Top-left
        {{ a/2,  b/2, c}, {1.f, 0.f, 1.f}}, // 5 Top-right
        {{ a/2, -b/2, c}, {0.f, 1.f, 1.f}}, // 6 Bottom-right
        {{-a/2, -b/2, c}, {0.f, 1.f, 1.f}}  // 7 Bottom-left
    },
    std::vector<GLuint>{
        // Front face
        0, 1, 3,   // top-left, top-right, bottom-left
        1, 2, 3,   // top-right, bottom-right, bottom-left

        // Back face (winding flipped for -z)
        4, 7, 5,   // top-left, bottom-left, top-right
        5, 7, 6,   // top-right, bottom-left, bottom-right

        // Left face
        0, 3, 4,   // front top-left, front bottom-left, back top-left
        4, 3, 7,   // back top-left, front bottom-left, back bottom-left

        // Right face
        1, 5, 2,   // front top-right, back top-right, front bottom-right
        5, 6, 2,   // back top-right, back bottom-right, front bottom-right

        // Bottom face
        3, 2, 7,   // front bottom-left, front bottom-right, back bottom-left
        2, 6, 7,   // front bottom-right, back bottom-right, back bottom-left

        // Top face
        0, 4, 1,   // front top-left, back top-left, front top-right
        1, 4, 5    // front top-right, back top-left, back top-right
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