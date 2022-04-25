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

void UI::renderInterface(SceneNodeUI* node, std::vector<AssetUI>* uiAssets)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    /*
    * Display demo window for reference?
    */
    bool show_demo_window = true;
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);


    //////////////////////
    //Create a UI window for scene objects
    //////////////////////
    ImGui::Begin("Scene");
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        processNode(node);
        ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
    //////////////////////
    
    //////////////////////
    //Create a UI window asset viewing
    //////////////////////
    ImGui::Begin("Assets");
    
    if (ImGui::Button("Add asset")) {
        ImGui::Text("Assets here");
    }
    for (auto const &ass : *uiAssets) {
        if (ass.type == Asset::AssetType::TEXTURE)
            ImGui::Text("Texture: %s",ass.name.c_str());
        if (ass.type == Asset::AssetType::MATERIAL)
            ImGui::Text("Material: %s",ass.name.c_str());
        if (ass.type == Asset::AssetType::MESH)
            ImGui::Text("Mesh: %s",ass.name.c_str());

    }
    ImGui::End();
    //////////////////////

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
    Locator::getLogger()->getLogger()->info(eventType);
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(ImGuiMouseButton_Left, true);
}

void UI::mousePosHandler(const char* eventType, float x, float y)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(x, y);
}

void UI::processNode(SceneNodeUI* node)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(node->id);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Object", "%s", node->name->c_str());

    if (node_open)
    {      
        float sliderSpeed = 0.01f;
        // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
        ImGui::TreeNodeEx("Position", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::DragFloat3("Position",&node->pos->x, sliderSpeed);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("Scale", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::DragFloat3("Scale", &node->scale->x, sliderSpeed);
        
        if (node->hasGraphics) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Material", flags);
            ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::Text("%s", node->material.c_str());

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Meshes", flags);
            for (auto const& mesh : node->meshes) {
                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::Text("%s", mesh.c_str());
            }
        }
        for (auto child : node->children)
        {
            processNode(&child);
        }
            
        ImGui::NextColumn();
        ImGui::PopID();
        ImGui::TreePop();
    }
    ImGui::PopID();
}
