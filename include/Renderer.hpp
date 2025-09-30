#pragma once
#include <vector>
#include <memory>
#include "Renderable.hpp"
#include "Camera.hpp"

class Renderer {
public:
    Renderer(Camera* cam);
    void draw(const std::vector<std::unique_ptr<Renderable>>& renderables);
    void update_aspect_ratio(float aspect);
private:
    Camera* camera; // not owning
    void uploadViewProjToShader(Shader& shader);
};
