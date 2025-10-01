#pragma once
#include <vector>
#include <memory>
#include "Renderable.hpp"
#include "Camera.hpp"

class Renderer {
    mutable bool dirty_renderables = false;
    std::vector<std::unique_ptr<Renderable>> cached_renderables;
public:
    Renderer(Camera* cam);
    void draw(std::vector<std::unique_ptr<Renderable>>& Renderables);
    void cache_draw(std::unique_ptr<Renderable> renderable);
    void update_aspect_ratio(float width, float height);
    void CommonDraw(Renderable* obj);
private:
    Camera* camera; // not owning
    void uploadViewProjToShader(Shader& shader);
};
