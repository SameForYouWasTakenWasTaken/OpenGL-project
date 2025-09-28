#include <glad/glad.h>

#include "Renderable.hpp"

static std::vector<Vertex> Create(size_t iter_num);

class Circle : public Renderable {
private:
    float radius;
public:
    Circle(float r, size_t iter_num, std::string& pathToShaderFile);

    void draw(GLenum usage) override;

    void update(float dt) override {};
    void BeforeDraw() override {}
};