#pragma once
#include "Renderable.hpp"

class Triangle : public Renderable {
    public:
    Triangle(std::vector<Vertex>& corner_points, std::string& pathToShaderSource);
    
    void BeforeDraw() override;
    
    void update(float dt) override {};
};