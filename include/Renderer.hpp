#pragma once
#include <vector>
#include <memory>
#include <queue>
#include "Renderable.hpp"
#include "Camera.hpp"

class Renderer {
public:
    std::vector<std::unique_ptr<Renderable>> cached_renderables;
    std::vector<std::shared_ptr<Renderable>> sh_cached_renderables; // Lets drawing renderables but not actually owning them (imGui support). Of course, you have to be wise with how you use this. Purely for ImGui support, but I doubt this'll be used anywhere else, soo...
    Renderer(Camera* cam);  
    void draw();
    void cache_draw(std::unique_ptr<Renderable> renderable);
    void cache_share_renderable(std::shared_ptr<Renderable> renderable);
    void CommonDraw(Renderable* obj);
    
    void update_aspect_ratio(float width, float height);
    void ChangeCamera(Camera* newCam);

    const Camera* GetCamera() const {return camera;}
    std::vector<Renderable*> GetAllRenderables() {
        std::vector<Renderable*> renderables;
        for (auto& r : cached_renderables) {
            renderables.push_back(r.get());
        }
        for (auto& r : sh_cached_renderables) {
            renderables.push_back(r.get());
        }
        return renderables;
    };
private:
    Camera* camera; // not owning, not needed. Your responsibility what you do with the camera in its state :)
    void uploadViewProjToShader(Shader& shader);
};
