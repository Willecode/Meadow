#include "ui.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include "service_locator/locator.h"
#include "input/inputevents.h"
#include "assets/asset.h"
UI::UI(): m_chosenAssetId(0), m_chosenAssetType(Asset::AssetType::TEXTURE)
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
    bool show_demo_window = false;
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
            if (ImGui::Selectable((std::to_string(ass.second.id) + " " + ass.second.name).c_str(), false)) {
                m_chosenAssetType = Asset::AssetType::TEXTURE;
                m_chosenAssetId = ass.first;
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Meshes")) {
        for (auto const& ass : uiAssets->meshes) {
            if (ImGui::Selectable((std::to_string(ass.second.id) + " " + ass.second.name).c_str(), false)) {
                m_chosenAssetType = Asset::AssetType::MESH;
                m_chosenAssetId = ass.first;
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Submeshes")) {
        for (auto const& ass : uiAssets->submeshes) {
            if (ImGui::Selectable((std::to_string(ass.second.id) + " " + ass.second.name).c_str(), false)) {
                m_chosenAssetType = Asset::AssetType::SUBMESH;
                m_chosenAssetId = ass.first;
            }
        }
        ImGui::TreePop();
    }
    if (ImGui::TreeNode("Materials")) {
        for (auto const& ass : uiAssets->materials) {
            if (ImGui::Selectable((std::to_string(ass.second.id) + " " + ass.second.name).c_str(), false)) {
                m_chosenAssetType = Asset::AssetType::MATERIAL;
                m_chosenAssetId = ass.first;
            }
        }
        ImGui::TreePop();
    }
    
    ImGui::EndChild();
    
    ImGui::SameLine();
    
    ImGui::BeginChild("AssetInspector", ImVec2(0, 260), true, window_flags);
    if (m_chosenAssetId != 0) {
        /*
        * If mesh chosen, display that meshes submeshes and their materials
        */
        if (m_chosenAssetType == Asset::AssetType::MESH) {
            for (auto const& smesh : uiAssets->meshes.at(m_chosenAssetId).submeshes) {
                ImGui::Text(uiAssets->submeshes.at(smesh.first).name.c_str());
                ImGui::SameLine();
                if (ImGui::BeginCombo("##materialcombo", uiAssets->materials.at(smesh.second).name.c_str())) {
                    for (auto const& mat : uiAssets->materials) {
                        if (ImGui::Selectable(mat.second.name.c_str(), false)) {
                            InputEvents::SetSubmeshMaterialEvent::notify(
                                m_chosenAssetId, smesh.first, mat.first);
                        }
                    }
                    ImGui::EndCombo();
                }
            }
        }
        /*
        * If material chosen...
        */
        if (m_chosenAssetType == Asset::AssetType::MATERIAL) {
            MaterialUI* chosenMat = &uiAssets->materials.at(m_chosenAssetId);

            /*
            * Diffuse map combobox
            */
            std::string diffComboLabel = "";
            if (chosenMat->diffuseMap == 0)
                diffComboLabel = "No texture";
            else
                diffComboLabel = uiAssets->textures.at(chosenMat->diffuseMap).name;
            if (ImGui::BeginCombo("Diffuse map", diffComboLabel.c_str())) {
                if (ImGui::Selectable("No texture", false)) {
                    InputEvents::setMaterialTextureEvent::notify(chosenMat->id,0, Texture::TextureType::DIFFUSE_MAP);
                }
                for (auto const& tex : uiAssets->textures) {
                    if (ImGui::Selectable(tex.second.name.c_str(), false)) {
                        InputEvents::setMaterialTextureEvent::notify(chosenMat->id, tex.first, Texture::TextureType::DIFFUSE_MAP);
                    }
                }
                ImGui::EndCombo();
                    
            }
            /*
            * Specular map combobox
            */
            std::string specComboLabel = "";
            if (chosenMat->specularMap == 0)
                specComboLabel = "No texture";
            else
                specComboLabel = uiAssets->textures.at(chosenMat->specularMap).name;
            if (ImGui::BeginCombo("Specular map", specComboLabel.c_str())) {
                for (auto const& tex : uiAssets->textures) {
                    if (ImGui::Selectable(tex.second.name.c_str(), false)) {
                        InputEvents::setMaterialTextureEvent::notify(chosenMat->id, tex.first, Texture::TextureType::SPECULAR_MAP);
                    }
                }
                ImGui::EndCombo();
            }

            /*
            * Material properties
            */
            for (auto& prop : *chosenMat->propertiesf) {
                ImGui::Text(prop.first.c_str());
                ImGui::DragFloat(prop.first.c_str(),&prop.second, 2);
            }
            for (auto& prop : *chosenMat->propertiesv3) {
                ImGui::Text(prop.first.c_str());
                ImGui::DragFloat3(prop.first.c_str(), &prop.second.r, 0.01);
            }
        }
    }
    
    ImGui::EndChild();

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

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("Rotation", flags);
        ImGui::TableSetColumnIndex(1);
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::DragFloat3("Rotation", &node->orientationEuler->x, sliderSpeed);

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        ImGui::TreeNodeEx("Mesh", flags);

        ImGui::TableSetColumnIndex(1);
        const char* meshboxlabel;
        if (node->mesh != nullptr)
            meshboxlabel = node->mesh->name.c_str();
        else
            meshboxlabel = "No Mesh";
        if (ImGui::BeginCombo("##meshbox", meshboxlabel)) {
            if (ImGui::Selectable("No Mesh", false))
            {
                InputEvents::SetNodeMeshEvent::notify(node->id, 0);
            }
            for (auto const& mesh : uiAssets->meshes) {
                if (ImGui::Selectable(mesh.second.name.c_str(), false))
                {
                    InputEvents::SetNodeMeshEvent::notify(node->id, mesh.second.id);
                }
            }
            ImGui::EndCombo();
        }
    
        for (auto child : node->children)
        {
            processNode(&child, uiAssets);
        }
        ImGui::TreePop();
    }
        ImGui::PopID();
}
