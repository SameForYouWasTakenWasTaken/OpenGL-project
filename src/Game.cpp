#include "Game.hpp"
#include "Functionalities/EBO.hpp"
#include "Functionalities/VBO.hpp"
#include "Functionalities/VAO.hpp"

#include "Renderables/Triangle.hpp"
#include "Renderables/Circle.hpp"
#include "ErrorReporting.hpp"

#include <vector>
#include <fstream>


bool Game::init(int width, int height){
    // Initialize GLFW
    WIDTH = width;
    HEIGHT = height;
    if(!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #if defined(_DEBUG) // Only in debug mode is debugging allowed. No shit.
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // Allow debugging
    #endif
    // Create a windowed mode window and its OpenGL context
    window = glfwCreateWindow(width, height, "OpenGL Window", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // Set callbacks
    glfwSetErrorCallback(error_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, [](GLFWwindow* win, int key, int scancode, int action, int mods){
        // Call your instance method
        Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(win));
        if (game) game->handleKey(key, action);
    });
    
    glfwSetWindowUserPointer(window, this);

    int version = gladLoadGL();
    if (version == 0) {
        return false;
    }

    enableReportGlErrors();
    spdlog::info("Loaded OpenGL: {}", version);
    // spdlog::info("GL Renderer: {}", glGetString(GL_RENDERER));
    // spdlog::info("GL Version: {}", glGetString(GL_VERSION));
    // spdlog::info("GL Shading Language Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void Game::run() {
    camera = new Camera();
    renderer = new Renderer(camera);

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.5f, 0.2f, 0.3f}},
        {{0.5f, -0.5f, 0.0f}, {1.f, 0.779f, 0.6f}},
        {{0.0f,  0.5f, 0.0f}, {0.1f, 0.3f, 0.9f}}
    };
    
    std::string standard_shader_path = "Shaders/Testing/standard.glsl";
    auto obj = std::make_unique<Triangle>(vertices, standard_shader_path);
    obj->SetPosition({0.f, 0.f, 0.f});
    obj->SetScale({2.f, 2.f, 1.f});

    // Now push it into the vector
    renderables.push_back(std::move(obj));

    // Main loop
    double lastFrame = 0.0;
    camera->lookAtTarget({0,0,0});
    while(!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        update(deltaTime);
        glClearColor(0.f, 0.f, 0.f, .0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        renderer->draw(renderables);
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void Game::update(float dt)
{
    float speed = 0.005f * dt;
    float rot_speed = 0.1 * dt;
    glm::vec3 move(0.f);
    glm::vec3 rot(0.f);

    // WASD/QE
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move.z += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move.z -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move.x += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) move.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) move.y += speed;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rot.x += rot_speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) rot.x -= rot_speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot.y += rot_speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot.y -= rot_speed;

    camera->Move(move);
    camera->Rotate(rot);
    
}

void Game::draw()
{
    
}

void Game::onResize(int width, int height)
{
    glViewport(0, 0, width, height);
    for (auto& rend : renderables) {

    }
}