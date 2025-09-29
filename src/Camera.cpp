#include "Camera.hpp"

Camera::Camera()
    : projType(ProjectionType::PERSPECTIVE),
      fovDeg(45.0f), nearPlane(0.1f), farPlane(500.0f),
      left_(-1), right_(1), bottom_(-1), top_(1),
      position({0,0,5}), front({0,0,0}), up({0,0,0}),
      dirtyView(true), dirtyProj(true), viewportWidth(800), viewportHeight(600)
{}

void Camera::setPerspective(float fov, float nearP, float farP) {
    projType = ProjectionType::PERSPECTIVE;
    fovDeg = fov;
    nearPlane = nearP;
    farPlane = farP;
    dirtyProj = true;
}


void Camera::setOrthographic(float left, float right, float bottom, float top, float nearP, float farP) {
    projType = ProjectionType::ORTHOGRAPHIC;
    left_ = left; right_ = right; bottom_ = bottom; top_ = top;
    nearPlane = nearP; farPlane = farP;
    dirtyProj = true;
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
    dirtyView = true;
}
void Camera::lookAtTarget(const glm::vec3& target, const glm::vec3& upHint) {
    worldUp = upHint;
    front = glm::normalize(target - position);
    // recompute yaw/pitch from front for consistency:
    // yaw = atan2(front.z, front.x) in degrees, pitch = asin(front.y)
    yaw = glm::degrees(atan2(front.z, front.x));
    pitch = glm::degrees(asin(glm::clamp(front.y, -1.0f, 1.0f)));
    updateVectors();
    dirtyView = true;
}

/// If caller already has a forward direction (unit) and wants to set it directly:
void Camera::setDirection(const glm::vec3& forward, const glm::vec3& upHint) {
    front = glm::normalize(forward);
    worldUp = upHint;
    // recompute right/up
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
    // optionally recompute yaw/pitch if you rely on them elsewhere
    yaw = glm::degrees(atan2(front.z, front.x));
    pitch = glm::degrees(asin(glm::clamp(front.y, -1.0f, 1.0f)));
    dirtyView = true;
}

void Camera::setViewportSize(int width, int height){ viewportWidth = width; viewportHeight = height; dirtyProj = true; }

const glm::mat4& Camera::getView() {
    if (dirtyView) rebuildView();
    return cachedView;
}

const glm::mat4& Camera::getProjection() {
    if (dirtyProj) rebuildProj(viewportWidth, viewportHeight);
    return cachedProj;
}

void Camera::rebuildView() {
    // position + front is target. Make sure front is normalized.
    cachedView = glm::lookAt(position, position + front, up);
    dirtyView = false;
}


void Camera::rebuildProj(int width, int height) {
    float aspect = width > 0 && height > 0 ? (float)width / (float)height : 4.0f/3.0f;
    if (projType == ProjectionType::PERSPECTIVE) {
        cachedProj = glm::perspective(glm::radians(fovDeg), aspect, nearPlane, farPlane);
    } else {
        // recompute ortho based on stored left/right/top/bottom scaled by aspect if needed
        cachedProj = glm::ortho(left_, right_, bottom_, top_, -1.0f, 1.0f);
    }
    dirtyProj = false;
}


void Camera::updateVectors() {
    // compute front from spherical coordinates using yaw/pitch (degrees)
    glm::vec3 f;
    f.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    f.y = sin(glm::radians(pitch));
    f.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(f);

    // recompute right/up using worldUp
    right = glm::normalize(glm::cross(front, worldUp));
    up    = glm::normalize(glm::cross(right, front));
}

void Camera::Move(const glm::vec3& delta) {
    // delta.x -> strafe, delta.y -> up, delta.z -> forward
    position += delta.x * right + delta.y * up + delta.z * front;
    dirtyView = true;
}

void Camera::Rotate(const glm::vec3& rotationDelta) {
    // rotationDelta: (pitchDelta, yawDelta, rollDelta?)
    pitch += rotationDelta.x;
    yaw   += rotationDelta.y;

    // clamp pitch
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    updateVectors();
    dirtyView = true;
}