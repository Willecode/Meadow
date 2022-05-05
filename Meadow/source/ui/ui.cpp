#include "ui.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "service_locator/locator.h"
#include "input/inputevents.h"
#include "assets/asset.h"
UI::UI()
{
}
UI::~UI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void UI::init(WindowManager* winMan)
{

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(winMan->getWindow(), true);
    ImGui_ImplOpenGL3_Init(NULL);

    /*
    * Subscribe to mouse button events
    */
    std::function<void()> mousefuncmr = std::bind(&UI::mouseButtonReleaseHandler, this);
    InputEvents::MouseButtonLeftReleasedEvent::subscribe(mousefuncmr);

    std::function<void()> mousefuncmp = std::bind(&UI::mouseButtonPressHandler, this);
    InputEvents::MouseButtonLeftPressedEvent::subscribe(mousefuncmp);

    /*
    * Subscribe to mouse move events
    */
    std::function<void(float, float)> mousefunc2 = std::bind(&UI::mousePosHandler, this, std::placeholders::_1, std::placeholders::_2);
    InputEvents::MouseMoveEvent::subscribe(mousefunc2);
}

void UI::renderInterface(SceneNodeUI* node, UIAssetMaps* uiAssets)
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
    if (ImGui::Button("Add node")) {
        InputEvents::AddNodeEvent::notify(0);
    }
    if (ImGui::BeginTable("split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable))
    {
        processNode(node, uiAssets);
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

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
    ImGui::BeginChild("AssetList", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), false, window_flags);
    if (ImGui::TreeNode("Textures")) {
        for (auto const& ass : uiAssets->textures) {
            ImGui::Selectable(ass.second.name.c_str());
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Meshes")) {
        for (auto const& ass : uiAssets->meshes) {
            ImGui::Selectable(ass.second.name.c_str());
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Submeshes")) {
        for (auto const& ass : uiAssets->submeshes) {
            ImGui::Selectable(ass.second.name.c_str());
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Materials")) {
        for (auto const& ass : uiAssets->materials) {
            ImGui::Selectable(ass.second.name.c_str());
        }
        ImGui::TreePop();
    }
    
    
    
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("AssetInspector", ImVec2(0, 260), true, window_flags);
    ImGui::Text("some stuff here");
    ImGui::Text("some stuff here");
    ImGui::Text("some stuff here");
    ImGui::Text("some stuff here");
    ImGui::EndChild();

    //ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
    //if (ImGui::BeginTable("split1", 1, ImGuiTableFlags_Resizable))
    //{
    //    for (int i = 0; i < 5; i++) {
    //        ImGui::Text("aaa");
    //        ImGui::TableNextColumn();
    //    }
    //    //for (auto const& ass : uiAssets->textures) {
    //    //    //ImGui::Text("Texture: %s", ass.second.name.c_str());
    //    //    ImGui::Selectable("Texture: %s", ass.second.name.c_str());
    //    //    ImGui::TableNextRow();
    //    //}
    //    //for (auto const& ass : uiAssets->meshes) {
    //    //    ImGui::Text("Mesh: %s", ass.second.name.c_str());
    //    //    ImGui::TableNextRow();
    //    //}
    //    //for (auto const& ass : uiAssets->submeshes) {
    //    //    ImGui::Text("Submesh: %s", ass.second.name.c_str());
    //    //    ImGui::TableNextRow();
    //    //}
    //    //for (auto const& ass : uiAssets->materials) {
    //    //    ImGui::Text("Material: %s", ass.second.name.c_str());
    //    //    ImGui::TableNextRow();
    //    //}
    //    ImGui::EndTable();
    //}

    //static bool selected[10] = {};
    //if (ImGui::BeginTable("split2", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
    //{
    //    for (int i = 0; i < 10; i++)
    //    {
    //        char label[32];
    //        sprintf(label, "Item %d", i);
    //        ImGui::TableNextColumn();
    //        ImGui::Selectable(label, &selected[i]); // FIXME-TABLE: Selection overlap
    //    }
    //    ImGui::EndTable();
    //}
   

    ImGui::End();
    //////////////////////

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::mouseButtonReleaseHandler()
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(ImGuiMouseButton_Left , false);
}

void UI::mouseButtonPressHandler()
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(ImGuiMouseButton_Left, true);
}

void UI::mousePosHandler(float x, float y)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMousePosEvent(x, y);
}

void UI::processNode(SceneNodeUI* node, UIAssetMaps* uiAssets)
{
    // Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    ImGui::PushID(node->uiElemId);

    // Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();

    if (ImGui::TreeNode("Object", "%s", node->name->c_str()))
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
        ImGui::DragFloat3("Position", &node->pos->x, sliderSpeed);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("Scale", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::DragFloat3("Scale", &node->scale->x, sliderSpeed);

        if (node->mesh != nullptr) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Mesh", flags);

            ImGui::TableSetColumnIndex(1);
            if (ImGui::BeginCombo("##meshbox", node->mesh->name.c_str())) {
                for (auto const& mesh : uiAssets->meshes) {
                    if (ImGui::Selectable(mesh.second.name.c_str(), false))
                    {
                        InputEvents::SetNodeMeshEvent::notify(node->id, mesh.second.id);
                    }
                }
                ImGui::EndCombo();
            }
        }
    
        for (auto child : node->children)
        {
            processNode(&child, uiAssets);
        }
        ImGui::TreePop();
    }
        ImGui::PopID();
}
