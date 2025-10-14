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
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Debug menu!");
    ImGui::SameLine();
    // Item spawning
    {
        const char* items[] = { "Triangle", "Circle", "Square" };
        static float col[4] = {1.f, 1.f, 1.f, 1};
        static float pos[3] = {0.f, 0.f, 0.f};
        static float orig[3] = { 0.f, 0.f, 0.f };
        static float rot[3] = { 0.f, 0.f, 0.f };
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
            ImGui::ColorEdit4("Pick a color!", col);
            ImGui::SliderFloat("Scale", &scale, 1.0f, 10.0f);
            ImGui::InputFloat3("Position", pos, "%.f");
            ImGui::InputFloat3("Origin", orig, "%.2f");
            ImGui::SeparatorText("Object Functionality");
        }
        if (item_current == 0) // Triangle
        {
            static float bottom_left = -0.5f;
            static float bottom_right = 0.5f;
            static float top_center = 0.0f;
            static float bottom_center = -.5f;
            static float z_axis = 0.0f;
            static float max = 10;
            ImGui::SliderFloat("Bottom left", &bottom_left, -max, max);
            ImGui::SliderFloat("Bottom right", &bottom_right, -max, max);
            ImGui::SliderFloat("Top center", &top_center, -max, max);
            ImGui::SliderFloat("Bottom center", &bottom_center, -max, max);
            ImGui::SliderFloat("Z axis", &z_axis, -max, max);

            std::vector<Vertex> vertices = {
                {{bottom_left, bottom_center, 0.0f}}, // Bottom left
                {{bottom_right, bottom_center, 0.0f}}, // Bottom right
                {{0.0f,  top_center, 0.0f}}, // Top center
                {{0.0f,  0.0f, z_axis}} // Z axis
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
                    t->SetPosition({ pos[0], pos[1], pos[2]});
                    t->SetColor({col[0], col[1], col[2], col[3]});
                    t->SetScale({scale, scale, scale});
                    t->SetIndices(indices);
                    t->SetOrigin({ orig[0], orig[1], orig[2]});
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
                    c->SetColor({col[0], col[1], col[2], col[3]});
                    c->SetScale({scale, scale, scale});
					c->SetOrigin({ orig[0], orig[1], orig[2] });

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
                    s->SetColor({col[0], col[1], col[2], col[3]});
                    s->SetScale({scale, scale, scale});
					s->SetOrigin({ orig[0], orig[1], orig[2] });

                    renderer->cache_share_renderable(s);
                    renderables.push_back(s);
                }
            }
        }
    }
    
    ImGui::SeparatorText("Camera settings");
    // Camera
    {
        if (!renderer->GetCamera()) return;
        static bool dirty_fov = false;
        static bool dirty_far = false;
        static bool dirty_sens = false;
        static bool dirty_speed = false;
        static bool dirty_lookAt = false;

        float fov = renderer->GetCamera()->fovDeg;
        float near = renderer->GetCamera()->nearPlane;
        float far = renderer->GetCamera()->farPlane;
        float sensitivity = renderer->GetCamera()->sensitivity;     
        
        static float lookAt[3];

        ImGui::SeparatorText("Camera");
        dirty_fov = ImGui::SliderFloat("FOV", &fov, 1.0f, 180.0f);
        dirty_far = ImGui::SliderFloat("Render distance", &far, 10.0f, 1000.0f);
        dirty_sens = ImGui::SliderFloat("Sensitivity", &sensitivity, 0.0f, 10.0f);
        dirty_lookAt = ImGui::InputFloat3("LookAt position", lookAt);

        if (dirty_fov)
            renderer->GetCamera()->setFOV(fov);
        if (dirty_far)
            renderer->GetCamera()->setFarPlane(far);
        if (dirty_sens)
            renderer->GetCamera()->setSensitivty(sensitivity);
        if (dirty_lookAt)
            renderer->GetCamera()->lookAtTarget(glm::vec3(lookAt[0], lookAt[1], lookAt[2]));
    }
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    ImGui::End();
    //ImGui::ShowDemoWindow();
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
