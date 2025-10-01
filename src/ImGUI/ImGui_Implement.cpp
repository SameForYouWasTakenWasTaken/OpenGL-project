#include "ImGui/ImGui_Implement.hpp"

void ImGui_Implement::Init(float main_scale, GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();  // local reference
    AddFlag(ImGuiConfigFlags_NavEnableKeyboard);
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void ImGui_Implement::AddFlag(ImGuiConfigFlags flag) {
    ImGuiIO& io = ImGui::GetIO();   // get it fresh each time
    io.ConfigFlags |= flag;
}

void ImGui_Implement::NewFrame() {
    if (!active) return;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Example window
    {
        ImGuiIO& io = ImGui::GetIO();
        static float f = 0.0f;
        static int counter = 0;
        static float pos[3];
        ImGui::Begin("Hello, world!");
        ImGui::Text("This is some useful text.");
        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::InputFloat3("Position", pos, "%.f");
        
        if (ImGui::Button("Press me to create me!") && renderer)
        {
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
             std::string path = "Shaders/Testing/standard.glsl";
             auto t = std::make_unique<Circle>(2.5, 64, path);
             t->SetPosition({pos[0], pos[1], pos[2]});
             spdlog::info("{}, {}, {}", pos[0], pos[1], pos[2]);

             renderer->cache_draw(std::move(t));
        }
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void ImGui_Implement::Render() {
    if (!active) return;
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGui_Implement::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete renderer;
}
