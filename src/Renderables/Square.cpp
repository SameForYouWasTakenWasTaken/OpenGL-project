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
			{{-a / 2, b / 2, 0.f}, { 1.f, 1.f, 1.f, 1 }}, // Top left
			{ { a / 2,  b / 2, 0.f}, {1.f, 1.f, 1.f, 1} }, // Top right
			{ { a / 2, -b / 2, 0.f}, {1.f, 1.f, 1.f, 1} }, // Bottom right
			{ {-a / 2, -b / 2, 0.f}, {1.f, 1.f, 1.f, 1} }, // Bottom left

			{ {-a / 2,  b / 2, c}, {1.f, 0.f, 1.f, 1} }, // 4 Top-left
			{ { a / 2,  b / 2, c}, {1.f, 0.f, 1.f, 1} }, // 5 Top-right
			{ { a / 2, -b / 2, c}, {0.f, 1.f, 1.f, 1} }, // 6 Bottom-right
			{ {-a / 2, -b / 2, c}, {0.f, 1.f, 1.f, 1} }  // 7 Bottom-left
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

Square::Square(std::vector<Vertex>& corner_points, std::vector<GLuint>& indices, std::string& pathToShadersource)
	: Renderable(ValidateCornerPoints(corner_points), indices)
{

	auto ShaderSources = ParseShaderFile(pathToShadersource);
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

	std::vector<Vertex> triangle_vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.5f, 0.2f, 0.3f, 1}}, // Bottom left
		{{0.5f, -0.5f, 0.0f}, {1.f, 0.779f, 0.6f, 1}}, // Bottom right
		{{0.0f,  0.5f, 0.0f}, {1.1f, 0.3f, 0.9f, 1}}, // Top center
	};
}
Square::Square(float a, float b, std::string& pathToShaderSource)

	: Renderable(
		std::vector<Vertex>{
			{{-a / 2, b / 2, 0.f}, { 1.f, 1.f, 1.f, 1 }}, // Top left
			{ { a / 2,  b / 2, 0.f}, {1.f, 1.f, 1.f, 1} }, // Top right
			{ { a / 2, -b / 2, 0.f}, {1.f, 1.f, 1.f, 1} }, // Bottom right
			{ {-a / 2, -b / 2, 0.f}, {1.f, 1.f, 1.f, 1} }, // Bottom left
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

Square::Square(float a, std::string& pathToShaderSource)
	: Square(a, a, pathToShaderSource) // Calls the rectangle constructor with equal sides
{
}

void Square::draw(GLenum usage)
{
	//SetIndices(indices);
	CommonDraw();
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
	GetVAO()->Unbind();
}