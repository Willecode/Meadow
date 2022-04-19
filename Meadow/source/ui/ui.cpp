#include "ui.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "service_locator/locator.h"
UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void UI::init(WindowManager* winMan, Dispatcher* disp)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(winMan->getWindow(), true);
    ImGui_ImplOpenGL3_Init(NULL);

    /*
    * Subscribe to mouse button events
    */
    std::function<void(const char*)> mousefuncmr = std::bind(&UI::mouseButtonReleaseHandler, this, std::placeholders::_1);
    disp->subscribe(MouseButtonLeftReleasedEvent::EVENT_TYPE, mousefuncmr);
    std::function<void(const char*)> mousefuncmp = std::bind(&UI::mouseButtonPressHandler, this, std::placeholders::_1);
    disp->subscribe(MouseButtonLeftPressedEvent::EVENT_TYPE, mousefuncmp);
    
    /*
    * Subscribe to mouse move events
    */
    std::function<void(const char*, float, float)> mousefunc2 = std::bind(&UI::mousePosHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    disp->subscribe2f(MouseMoveEvent::EVENT_TYPE, mousefunc2);

}

void UI::renderInterface()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
    ImGui::Checkbox("Another Window", &show_another_window);

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::mouseButtonReleaseHandler(const char* eventType)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(ImGuiMouseButton_Left , false);


}

void UI::mouseButtonPressHandler(const char* eventType)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(ImGuiMouseButton_Left, true);
}

void UI::mousePosHandler(const char* eventType, float x, float y)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(x, y);
}
