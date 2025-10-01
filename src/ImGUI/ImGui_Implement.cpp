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
        ImGui::Begin("Hello, world!");

        ImGui::SameLine();
        {
            const char* items[] = { "Triangle", "Circle", "Square" };
            static float col[3];
            static float pos[3] = {0.f, 0.f, 0.f};
            static float scale = 1.0f;
            static std::string p = "Shaders/Testing/standard.glsl";
            static int item_current = 1;
            // Simple selection popup (if you want to show the current selection inside the Button itself,
            // you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
            if (ImGui::Button("Select object!"))
                ImGui::OpenPopup("my_select_popup");
            ImGui::SameLine();
            ImGui::TextUnformatted(item_current == -1 ? "<None>" : items[item_current]);
            if (ImGui::BeginPopup("my_select_popup"))
            {
                ImGui::SeparatorText("Shapes");
                for (int i = 0; i < IM_ARRAYSIZE(items); i++)
                    if (ImGui::Selectable(items[i]))
                        item_current = i;
                ImGui::EndPopup();
            }

            if (item_current >= 0)
            {
                ImGui::ColorEdit3("Pick a color!", col);
                ImGui::SliderFloat("Scale", &scale, 1.0f, 10.0f);
                ImGui::InputFloat3("Position", pos, "%.f");
                ImGui::SeparatorText("Functionality");
            }
            if (item_current == 0) // Triangle
            {
                std::vector<Vertex> vertices = {
                    {{-0.5f, -0.5f, 0.0f}}, // Bottom left
                    {{0.5f, -0.5f, 0.0f}}, // Bottom right
                    {{0.0f,  0.5f, 0.0f}}, // Top center
                    {{0.0f,  0.0f, 0.5f}} // Z axis
                };
                std::vector<unsigned int> indices = {
                    0, 1, 2, // base
                    0, 3, 1, // side
                    1, 3, 2, // side
                    2, 0, 3  // side
                };
                if (renderer)
                {
                    if (ImGui::Button("Run me!"))
                    {
                        auto t = std::make_shared<Triangle>(vertices, p);
                        t->SetPosition({pos[0], pos[1], pos[2]});
                        t->SetColor({col[0], col[1], col[2]});
                        t->SetScale({scale, scale, scale});
                        t->SetIndices(indices);
                        renderer->cache_share_renderable(t);
                        renderables.push_back(t);
                    }
                }
            } else if (item_current == 1) // Circle
            {
                if (renderer)
                {
                    static float radius = 1.0f;
                    ImGui::SliderFloat("Radius", &radius, 1.0f, 10.0f, "%.2f");
                    if (ImGui::Button("Run me!"))
                    {

                        auto c = std::make_shared<Circle>(radius, 32, p);
                        c->SetPosition({pos[0], pos[1], pos[2]});
                        c->SetColor({col[0], col[1], col[2]});
                        c->SetScale({scale, scale, scale});
                        renderer->cache_share_renderable(c);
                        renderables.push_back(c);
                    }
                }
            } else if (item_current == 2) // Square
            {
                static float width = 1.f;
                static float height = 1.f;
                static float depth = 1.f;
                ImGui::InputFloat("Width", &width);
                ImGui::InputFloat("Height", &height);
                ImGui::InputFloat("Depth", &depth);
                if (renderer)
                {
                    if (ImGui::Button("Run me!"))
                    {
                        auto s = std::make_shared<Square>(width, height, depth, p);
                        s->SetPosition({pos[0], pos[1], pos[2]});
                        s->SetColor({col[0], col[1], col[2]});
                        s->SetScale({scale, scale, scale});
                        renderer->cache_share_renderable(s);
                        renderables.push_back(s);
                    }
                }
            }

        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
        ImGui::ShowDemoWindow();
    }
}

void ImGui_Implement::Render() {
    if (!active) return;
    if (renderer)
    {

    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void ImGui_Implement::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    delete renderer;
}
