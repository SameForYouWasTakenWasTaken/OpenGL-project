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
    
    imgui = new ImGui_Implement(); // Create ImGui handler

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_DEPTH_BITS, 24); // Allow a depth buffer

    #if defined(_DEBUG) // Only in debug mode is debugging allowed. No shit.
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true); // Allow debugging
    #endif

    // Create a windowed mode window and its OpenGL context, alongside ImGui
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
    window = glfwCreateWindow(width, height, "OpenGL window", nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1); // VSync
    glfwSwapInterval(0); // No sync
    

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
    // glEnable(GL_CULL_FACE); // enable face culling
    glEnable(GL_DEPTH_TEST); // ensures correct depth rendering
	glEnable(GL_BLEND); // Enables blending
    glDepthFunc(GL_LESS); // default: pass if fragment is closer
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Blending alpha thingy. Basically lets stuff be opaque or not

    spdlog::info("Loaded OpenGL: {}", version);
    // spdlog::info("GL Renderer: {}", glGetString(GL_RENDERER));
    // spdlog::info("GL Version: {}", glGetString(GL_VERSION));
    // spdlog::info("GL Shading Language Version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    // Setup ImGui
    imgui->Init(main_scale, window);
    return true;
}

void Game::mousePosCallback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos); // forward to ImGui
    Game* game = reinterpret_cast<Game*>(glfwGetWindowUserPointer(window));
    if (game && game->camera) {
        game->camera->mousePosCallback(window, xpos, ypos);
    }
}

void Game::run() {
    camera = new Camera();
    renderer = new Renderer(camera);
    imgui->AssignRenderer(renderer);

    glfwSetCursorPosCallback(window, mousePosCallback);

    std::vector<Vertex> triangle_vertices = {
        {{-0.5f, -0.5f, 0.0f}, {1.5f, 0.2f, 0.3f, 1}}, // Bottom left
        {{0.5f, -0.5f, 0.0f}, {1.f, 0.779f, 0.6f, 1}}, // Bottom right
        {{0.0f,  0.5f, 0.0f}, {1.1f, 0.3f, 0.9f, 1}}, // Top center
        {{0.0f,  0.0f, 1.0f}, {0.5f, 0.1f, 0.3f, 1}} // Z axis
    };
    std::vector<unsigned int> triangle_indices = {
        0, 1, 2, // base
        0, 3, 1, // side
        1, 3, 2, // side
        2, 0, 3  // side
    };
    
    // Cube
    const float a = 5;
    const float b = 5;
    const float c = 5;


    std::string standard_shader_path = "Shaders/Testing/standard.glsl";
    auto obj = std::make_unique<Circle>(5.f, 64, standard_shader_path);
    obj->SetPosition({0.f, 0.f, 0.f});
    obj->SetScale({1.f, 1.f, 3.f});

    auto obj2 = std::make_unique<Triangle>(triangle_vertices, standard_shader_path);
    obj2->SetIndices(triangle_indices);
    obj2->SetPosition({10.f, 0.f, 0.f});
    obj2->SetScale({5.f,5.f,10.f});

    auto obj3 = std::make_unique<Square>(a, b, c, standard_shader_path);
    obj3->SetPosition({-10.f, 0.f, 0.f});
    obj3->SetScale({2.f,2.f,2.f});

    obj->SetColor({ 1,1,1, 1});

    // Now push it into the vector
    renderer->cache_draw(std::move(obj3));
    renderer->cache_draw(std::move(obj));
    renderer->cache_draw(std::move(obj2));

    // Main loop
    double lastFrame = glfwGetTime();
    camera->lookAtTarget({0,0,0});
    camera->setFOV(90);
    while(!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        float deltaTime = static_cast<float>(currentFrame - lastFrame);
        lastFrame = currentFrame;

        imgui->NewFrame();
        update(deltaTime);
        glClearColor(0.f, 0.f, 0.f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer->draw();
        imgui->Render();
        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void Game::update(float dt)
{
    const float speed = 10.f * dt;
    const float rot_speed = 60.f * dt;
    glm::vec3 move(0.f);
    glm::vec3 rot(0.f);
    // WASD/QE
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) move.z += speed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) move.z -= speed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) move.x -= speed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) move.x += speed;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) move.y -= speed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) move.y += speed;

    // Rotation with mouse
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Disable mouse cursor
        camera->ActivateCursorMovement();
        
    }else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Enable mouse cursor
        camera->DeactivateCursorMovement();
    }

    camera->Move(move);
    camera->Rotate(rot);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        for (auto& renderable : renderer->GetAllRenderables()) {
            renderable->Rotate(50 * dt, ROTATION::NORTH);
        }
    } else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        spdlog::info("{}", renderer->GetAllRenderables().size());
    }
    
}

void Game::draw()
{
    
}

void Game::onResize(int width, int height)
{
    glViewport(0, 0, width, height);
    renderer->update_aspect_ratio(static_cast<float>(width), static_cast<float>(height));
}