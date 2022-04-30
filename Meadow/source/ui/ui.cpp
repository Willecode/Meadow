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

void UI::renderInterface(SceneNodeUI* node, std::map<Asset::AssetType, UI::UIAssetMap>* uiAssets)
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
    
    if (ImGui::Button("Add asset")) {
        ImGui::Text("Assets here");
    }
    for (auto const &ass : uiAssets->at(Asset::AssetType::TEXTURE)) {
            ImGui::Text("Texture: %s",ass.second.name.c_str());
    }
    for (auto const &ass : uiAssets->at(Asset::AssetType::MESH)) {
            ImGui::Text("Mesh: %s",ass.second.name.c_str());
    }
    for (auto const &ass : uiAssets->at(Asset::AssetType::MATERIAL)) {
            ImGui::Text("Material: %s",ass.second.name.c_str());
    }

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

void UI::processNode(SceneNodeUI* node, std::map<Asset::AssetType, UI::UIAssetMap>* uiAssets)
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

        if (node->hasGraphics) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Material", flags);

            /*
            * Material combobox
            */
            ImGui::TableSetColumnIndex(1);
            if (ImGui::BeginCombo("##material", node->material->name.c_str()))
            {
                for (auto const& material : uiAssets->at(Asset::AssetType::MATERIAL)) {
                    if (ImGui::Selectable(material.second.name.c_str(), false))
                    {
                        InputEvents::SetNodeMaterialEvent::notify(node->id, material.second.id);
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::AlignTextToFramePadding();
            ImGui::TreeNodeEx("Mesh", flags);

            /*
            * Mesh combobox
            */
            ImGui::TableSetColumnIndex(1);
            if (ImGui::BeginCombo("##mesh", node->mesh->name.c_str())) // Empty or an existing label makes the CB disabled
            {
                for (auto const& mesh : uiAssets->at(Asset::AssetType::MESH)) {
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
