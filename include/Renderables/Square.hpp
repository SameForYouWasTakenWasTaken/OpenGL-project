#pragma once
#include "Renderable.hpp"

// Can also be used as a rectangle! :)
class Square : public Renderable
{
private:


public:
    Square(std::vector<Vertex>& corner_points, std::vector<GLuint>& indices, std::string& pathToShaderSource);
    Square(float a, std::string& pathToShaderSource); // Function creates a square with each side length of a
    Square(float a, float b, std::string& pathToShaderSource); // Function creates a rectangle with width a and height b

    void BeforeDraw() override {};
    void update(float dt) override {};
    void draw(GLenum usage) override;
};