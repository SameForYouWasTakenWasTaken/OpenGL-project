#include "Renderer.hpp"

void Renderer::CommonDraw(Renderable* obj)
{
    if (!obj->available_shader()) 
        obj->create_shaders();
        
        Shader* sh = obj->GetShader();
        if (sh) {
            uploadViewProjToShader(*sh);
        }
        obj->draw(GL_TRIANGLES); // this should set 'model' uniform (and any object-local uniforms)
        obj->GetVAO()->Unbind(); // To solve confusion, and errors within draw functions, unbind VAO after no use needed
}

Renderer::Renderer(Camera* cam) : camera(cam) {}

void Renderer::uploadViewProjToShader(Shader& shader) {
    shader.CacheUniforms();
    glm::mat4 proj = camera->getProjection();
    glm::mat4 view = camera->getView();
    glUniformMatrix4fv(shader.locProjection, 1, GL_FALSE, glm::value_ptr(proj));
    glUniformMatrix4fv(shader.locView, 1, GL_FALSE, glm::value_ptr(view));
}

void Renderer::update_aspect_ratio(float width, float height) {
    if (camera) {
        camera->setViewportSize(static_cast<int>(width), static_cast<int>(height));
    }
}

void Renderer::draw(std::vector<std::unique_ptr<Renderable>>& renderables) {
    
    // Add the cached renderables to the original renderables vector
    if (dirty_renderables) {
        for (auto& obj : cached_renderables)
            renderables.push_back(std::move(obj));
        cached_renderables.clear();    
    }
    dirty_renderables = false;
    
    spdlog::info("{}, {}",renderables.size(), cached_renderables.size());

    for (auto& obj : renderables)
        CommonDraw(obj.get());
}


void Renderer::cache_draw(std::unique_ptr<Renderable> r)
{
    cached_renderables.push_back(std::move(r));
    dirty_renderables = true;
}