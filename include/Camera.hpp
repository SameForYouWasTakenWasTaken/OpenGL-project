#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:


// camera transform
glm::vec3 position;
glm::vec3 front;
glm::vec3 up;
glm::vec3 right;
glm::vec3 worldUp;


int viewportWidth, viewportHeight;

// cache
mutable glm::mat4 cachedView;
mutable glm::mat4 cachedProj;
// Flags that say if the cache is outdated and need recomputing
mutable bool dirtyView;
mutable bool dirtyProj;

float lastMouseX, lastMouseY;
bool cursor_movement = false;
bool firstMouseInput = true; // Will be set to true later :)

void rebuildProj(int width, int height);
void rebuildView();
void updateVectors();

public:
// perspective params
float fovDeg;
float nearPlane;
float farPlane;
float sensitivity;

float yaw;   // horizontal rotation (around Y)
float pitch; // vertical rotation (around X)
float fov;   // for perspective

Camera();


void Move(const glm::vec3& delta);
void Rotate(const glm::vec3& rotation);
void ActivateCursorMovement() { cursor_movement = true; };
void DeactivateCursorMovement() { cursor_movement = false; };
// configuration
void setPerspective(float fovDeg, float nearPlane, float farPlane);
void setPosition(const glm::vec3& pos);
void lookAtTarget(const glm::vec3& target, const glm::vec3& up = {0,1,0});
void setViewportSize(int width, int height); // call on resize
void setDirection(const glm::vec3& forward, const glm::vec3& upHint);
void setAspectRatio(float aspect);
void setFOV(float fovDeg);
void setNearPlane(float nearPlane);
void setFarPlane(float farPlane);
void setViewport(int width, int height);
void setSensitivty(float sens);

void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
// Read-functions
const glm::mat4& getView();
const glm::mat4& getProjection();

};
