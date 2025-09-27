#pragma once
#include<iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "Renderable.hpp"

template<typename... Args>
void display_err_message(const Args&... msgs)
{
    (spdlog::error("{}", msgs), ...);
    ((std::cerr << msgs << std::endl), ...);
}

class Game {
    std::vector<std::unique_ptr<Renderable>> renderables;
    GLFWwindow* window;
    unsigned int WIDTH;
    unsigned int HEIGHT;

    static void error_callback(int code, const char* description) {
        display_err_message(code, description);
    }
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }
    void handleKey(int key, int action)
    {
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
        {
            for (auto& renderable : renderables)
            {
                
            }
        }
    }


public:

    bool init(int width, int height);
    void run();
    void update(float dt);
    void draw();
};