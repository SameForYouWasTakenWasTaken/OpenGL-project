#include "Game.hpp"
#include "Functionalities/EBO.hpp"
#include "Functionalities/VBO.hpp"
#include "Functionalities/VAO.hpp"

#include "Renderable.hpp"

#include <vector>
#include <fstream>

bool Game::init(int width, int height) {
    // Initialize GLFW
    WIDTH = width;
    HEIGHT = height;
    if(!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    spdlog::info("Loaded OpenGL: {}", version);
    // spdlog::info("GL Renderer: {}", glGetString(GL_RENDERER));
    // spdlog::info("GL Version: {}", glGetString(GL_VERSION));
    // spdlog::info("GL Shading Language Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void Game::run() {

    std::vector<glm::vec3> vertices = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.0f,  0.5f, 0.0f}
    };

    auto ShaderSources = ParseShaderFile("Shaders/Testing/triangle.glsl");    

    VAOattrib vertex;
    vertex.layout = 0;
    vertex.numComponents = 3;
    vertex.stride = 3;

    std::vector<GLuint> indices = {0, 1, 2};

    auto triangle = std::make_unique<Renderable>(vertices, indices, std::vector<VAOattrib>{vertex});

    // Set up shaders separately
    triangle->set_shader_sources(ShaderSources.FragmentSource, ShaderSources.VertexSource);
    triangle->create_shaders();
    // Now push it into the vector
    renderables.push_back(std::move(triangle));

    // Main loop
    double lastFrame = 0.0;
    while(!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        
        update(deltaTime);
        glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        draw();
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void Game::update(float dt)
{

}

void Game::draw()
{
    for (const auto& renderable : renderables)
    {
        renderable->draw(GL_TRIANGLES);
    }
}