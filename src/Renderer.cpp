#include "Renderer.hpp"

#include <iostream>

void Renderer::CommonDraw(Renderable* obj)
{
    if (!obj->available_shader()) 
        obj->create_shaders();
        
        Shader* sh = obj->GetShader();
        if (sh) {
            uploadViewProjToShader(*sh);
        }
        obj->draw(GL_TRIANGLES); // this should set 'model' uniform (and any object-local uniforms)
        obj->GetVAO()->Unbind(); // To solve confusion, and errors within draw functions, unbind VAO after no use needed. This does not mean you 'should' unbind VAO yourself, it's purely automatic, that is what I'm hoping to achieve with the VAO, VBO and EBO system. Should be purely automatic, no need to worry about it :)
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

void Renderer::draw() {
    for(auto& obj : cached_renderables)
        CommonDraw(obj.get());
    for(auto& obj : sh_cached_renderables)
        CommonDraw(obj.get());}


void Renderer::cache_draw(std::unique_ptr<Renderable> r)
{
    cached_renderables.push_back(std::move(r));
}

void Renderer::cache_share_renderable(std::shared_ptr<Renderable> renderable)
{
    sh_cached_renderables.push_back(renderable);
}

void Renderer::ChangeCamera(Camera* newCam)
{
    camera = newCam;
}