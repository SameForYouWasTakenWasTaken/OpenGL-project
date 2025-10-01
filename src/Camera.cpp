#include "Camera.hpp"


Camera::Camera()
: fovDeg(45.0f), nearPlane(0.1f), farPlane(500.0f),
position({0,0,5}), front({0,0,-1}), up({0,1,0}),
worldUp({0,1,0}),
yaw(-90.0f), pitch(0.0f), fov(45.0f),
viewportWidth(800), viewportHeight(600),
dirtyView(true), dirtyProj(true)
{}

void Camera::setPerspective(float fov, float nearP, float farP) {
    fovDeg = fov;
    nearPlane = nearP;
    farPlane = farP;
    dirtyProj = true;
}

void Camera::setFOV(float fovDeg) {
    this->fovDeg = fovDeg;
    dirtyProj = true;
}

void Camera::setNearPlane(float nearPlane) {
    this->nearPlane = nearPlane;
    dirtyProj = true;
}

void Camera::setFarPlane(float farPlane) {
    this->farPlane = farPlane;
    dirtyProj = true;
}

void Camera::setViewport(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    dirtyProj = true;
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
    dirtyView = true;
}

void Camera::lookAtTarget(const glm::vec3& target, const glm::vec3& upHint) {
    worldUp = upHint;
    front = glm::normalize(target - position);
    yaw = glm::degrees(atan2(front.z, front.x));
    pitch = glm::degrees(asin(glm::clamp(front.y, -1.0f, 1.0f)));
    updateVectors();
    dirtyView = true;
}

void Camera::setDirection(const glm::vec3& forward, const glm::vec3& upHint) {
    front = glm::normalize(forward);
    worldUp = upHint;
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
    yaw = glm::degrees(atan2(front.z, front.x));
    pitch = glm::degrees(asin(glm::clamp(front.y, -1.0f, 1.0f)));
    dirtyView = true;
}

void Camera::setViewportSize(int width, int height) {
    viewportWidth = width;
    viewportHeight = height;
    dirtyProj = true;
}

const glm::mat4& Camera::getView() {
    if (dirtyView) rebuildView();
    return cachedView;
}

const glm::mat4& Camera::getProjection() {
    if (dirtyProj) rebuildProj(viewportWidth, viewportHeight);
    return cachedProj;
}

void Camera::rebuildView() {
    cachedView = glm::lookAt(position, position + front, up);
    dirtyView = false;
}

void Camera::rebuildProj(int width, int height) {
    float aspect = width > 0 && height > 0 ? (float)width / (float)height : 4.0f/3.0f;
    cachedProj = glm::perspective(glm::radians(fovDeg), aspect, nearPlane, farPlane);
    dirtyProj = false;
}

void Camera::updateVectors() {
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);


    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));


}

void Camera::Move(const glm::vec3& delta) {
    position += delta.x * right + delta.y * up + delta.z * front;
    dirtyView = true;
}

void Camera::Rotate(const glm::vec3& rotationDelta) {
    pitch += rotationDelta.x;
    yaw += rotationDelta.y;


    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
    dirtyView = true;
}

void Camera::setAspectRatio(float aspect) {
    cachedProj = glm::perspective(glm::radians(fovDeg), aspect, nearPlane, farPlane);
    dirtyProj = false;
}
