#pragma once
#include<iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "Renderable.hpp"
#include "Renderer.hpp"
#include "Camera.hpp"

#include "ImGUI/ImGui_Implement.hpp"
template<typename... Args>
void display_err_message(const Args&... msgs)
{
	(spdlog::error("{}", msgs), ...);
	((std::cerr << msgs << std::endl), ...);
}

class Game {
	GLFWwindow* window;
	Camera* camera;
	Renderer* renderer;

#ifdef ENABLE_IMGUI
	ImGui_Implement* imgui;
#endif

	std::vector<std::unique_ptr<Renderable>> renderables;
	unsigned int WIDTH;
	unsigned int HEIGHT;


	static void error_callback(int code, const char* description) {
		display_err_message(code, description);
	}
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
		if (game) {
			game->onResize(width, height);
		}
	}
	void handleKey(int key, int action)
	{

	}

	void onResize(int width, int height);
	static void mousePosCallback(GLFWwindow* window, double xpos, double ypos);
public:
	bool init(int width, int height);
	void run();
	void update(float dt);
	void draw();
};