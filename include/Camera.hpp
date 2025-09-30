#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class ProjectionType {ORTHOGRAPHIC, PERSPECTIVE};

class Camera {
private:
    ProjectionType projType;
    // perspective params
    float fovDeg;
    float nearPlane;
    float farPlane;

    // ortho params
    float left_, right_, bottom_, top_;

    // camera transform
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    float yaw;   // horizontal rotation (around Y)
    float pitch; // vertical rotation (around X)
    float fov;   // for perspective
    int viewportWidth, viewportHeight;
    
    // cache
    mutable glm::mat4 cachedView;
    mutable glm::mat4 cachedProj;
    mutable bool dirtyView;
    mutable bool dirtyProj;
    
    void rebuildProj(int width, int height);
    void rebuildView();
    void updateVectors();
public:
    Camera();

    void Move(const glm::vec3& delta); // moves both position and target
    void Rotate(const glm::vec3& Rotation);

    // configuration
    void setPerspective(float fovDeg, float nearPlane, float farPlane);
    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
    void setPosition(const glm::vec3& pos);
    void lookAtTarget(const glm::vec3& target, const glm::vec3& up = {0,1,0});
    void setViewportSize(int width, int height); // call on resize
    void setDirection(const glm::vec3& forward, const glm::vec3& upHint);
    void setAspectRatio(float aspect); // modifies the projection matrix
    // Read-functions
    const glm::mat4& getView();
    const glm::mat4& getProjection();
};