#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Renderer.hpp"
#include "Renderables/Triangle.hpp"
#include "Renderables/Circle.hpp"

class ImGui_Implement {
    bool active = true;
    Renderer* renderer = nullptr; // not owning
public:
    ~ImGui_Implement()
    {
        Shutdown();
    };
    void Init(float main_scale, GLFWwindow* window);
    void NewFrame();
    void AddFlag(ImGuiConfigFlags flag);
    void Render();
    void Shutdown();

    void Activate() { active = true; }
    void Deactivate() { active = false; }

    void AssignRenderer(Renderer* r){ renderer = r;}
    Renderer* GetRenderer() { return renderer; }
};
