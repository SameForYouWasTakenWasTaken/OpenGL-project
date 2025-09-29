#include "Game.hpp"
#include "Functionalities/EBO.hpp"
#include "Functionalities/VBO.hpp"
#include "Functionalities/VAO.hpp"

#include "Renderables/Triangle.hpp"
#include "Renderables/Circle.hpp"
#include "Renderables/Square.hpp"
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
    glfwWindowHint(GLFW_DEPTH_BITS, 24); // Allow a depth buffer

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

    // OpenGL initialization (after creating context)
    glEnable(GL_DEPTH_TEST); // ensures correct depth rendering
    // glEnable(GL_CULL_FACE); // enable face culling
    // glCullFace(GL_BACK); // cull back-facing triangles
    // glFrontFace(GL_CCW); // define counter-clockwise triangles as front faces
    glDepthFunc(GL_LESS); // default: pass if fragment is closer

    spdlog::info("Loaded OpenGL: {}", version);
    // spdlog::info("GL Renderer: {}", glGetString(GL_RENDERER));
    // spdlog::info("GL Version: {}", glGetString(GL_VERSION));
    // spdlog::info("GL Shading Language Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void Game::run() {
    camera = new Camera();
    renderer = new Renderer(camera);

    std::vector<Vertex> triangle_vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.5f, 0.2f, 0.3f}}, // Bottom left
        {{0.5f, -0.5f, 0.0f}, {1.f, 0.779f, 0.6f}}, // Bottom right
        {{0.0f,  0.5f, 0.0f}, {1.1f, 0.3f, 0.9f}}, // Top center
        {{0.0f,  0.0f, 1.0f}, {0.5f, 0.1f, 0.3f}} // Z axis
    };
    std::vector<unsigned int> triangle_indices = {
        0, 1, 2, // base
        0, 3, 1, // side
        1, 3, 2, // side
        2, 0, 3  // side
    };
    
    float a = 5;
    float b = 5;
    float c = 5;
   
    std::vector<Vertex> cube_vertices = {
        // FRONT FACE (z = 0)
        {{-a/2,  b/2, 0.0f}, {1.f, 1.f, 0.f}}, // 0 Top-left
        {{ a/2,  b/2, 0.0f}, {1.f, 1.f, 0.f}}, // 1 Top-right
        {{ a/2, -b/2, 0.0f}, {1.f, 1.f, 1.f}}, // 2 Bottom-right
        {{-a/2, -b/2, 0.0f}, {1.f, 1.f, 1.f}}, // 3 Bottom-left

        // BACK FACE (z = c)
        {{-a/2,  b/2, c}, {1.f, 0.f, 1.f}}, // 4 Top-left
        {{ a/2,  b/2, c}, {1.f, 0.f, 1.f}}, // 5 Top-right
        {{ a/2, -b/2, c}, {0.f, 1.f, 1.f}}, // 6 Bottom-right
        {{-a/2, -b/2, c}, {0.f, 1.f, 1.f}}  // 7 Bottom-left
    };

    std::vector<GLuint> cube_indices = {
        // Front face
        0, 1, 3,   // top-left, top-right, bottom-left
        1, 2, 3,   // top-right, bottom-right, bottom-left

        // Back face (winding flipped for -z)
        4, 7, 5,   // top-left, bottom-left, top-right
        5, 7, 6,   // top-right, bottom-left, bottom-right

        // Left face
        0, 3, 4,   // front top-left, front bottom-left, back top-left
        4, 3, 7,   // back top-left, front bottom-left, back bottom-left

        // Right face
        1, 5, 2,   // front top-right, back top-right, front bottom-right
        5, 6, 2,   // back top-right, back bottom-right, front bottom-right

        // Bottom face
        3, 2, 7,   // front bottom-left, front bottom-right, back bottom-left
        2, 6, 7,   // front bottom-right, back bottom-right, back bottom-left

        // Top face
        0, 4, 1,   // front top-left, back top-left, front top-right
        1, 4, 5    // front top-right, back top-left, back top-right
    };


    std::string standard_shader_path = "Shaders/Testing/standard.glsl";
    auto obj = std::make_unique<Circle>(5.f, 8, standard_shader_path);
    obj->SetPosition({0.f, 0.f, 0.f});
    obj->SetScale({1.f, 1.f, 3.f});

    auto obj2 = std::make_unique<Triangle>(triangle_vertices, standard_shader_path);
    obj2->SetIndices(triangle_indices);
    obj2->SetPosition({10.f, 0.f, 0.f});
    obj2->SetScale({5.f,5.f,10.f});

    auto obj3 = std::make_unique<Square>(cube_vertices, cube_indices, standard_shader_path);
    obj3->SetPosition({-10.f, 0.f, 0.f});
    obj3->SetScale({2.f,2.f,2.f});

    // Now push it into the vector
    renderables.push_back(std::move(obj));
    renderables.push_back(std::move(obj2));
    renderables.push_back(std::move(obj3));

    // Main loop
    double lastFrame = glfwGetTime();
    camera->lookAtTarget({0,0,0});
    while(!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;
        update(deltaTime);
        glClearColor(0.f, 0.f, 0.f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->draw(renderables);
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void Game::update(float dt)
{
    float speed = 10.f * dt;
    float rot_speed = 60.f * dt;
    glm::vec3 move(0.f);
    glm::vec3 rot(0.f);
    // WASD/QE
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move.z += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move.z -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move.x += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) move.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) move.y += speed;

    // Rotation
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) rot.x += rot_speed;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) rot.x -= rot_speed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot.y += rot_speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot.y -= rot_speed;

    camera->Move(move);
    camera->Rotate(rot);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        for (auto& renderable : renderables) {
            renderable->Rotate(50 * dt, ROTATION::NORTH);
        }
    }
    
}

void Game::draw()
{
    
}

void Game::onResize(int width, int height)
{
    glViewport(0, 0, width, height);
}