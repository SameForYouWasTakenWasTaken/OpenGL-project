#include "Renderer.hpp"

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

void Renderer::draw(const std::vector<std::unique_ptr<Renderable>>& renderables) {
    for (const auto& obj : renderables) {
        if (!obj->available_shader()) 
            obj->create_shaders();
        
        Shader* sh = obj->GetShader();
        if (sh) {
            uploadViewProjToShader(*sh);
        }
        obj->draw(GL_TRIANGLES); // this should set 'model' uniform (and any object-local uniforms)
    }
}
