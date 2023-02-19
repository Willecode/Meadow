#include "ui.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
//#include <backends/imgui_impl_opengl3.h>
//#include "service_locator/locator.h"
#include "assets/asset.h"
#include "shader/shadermanager.h"
#include "input/inputevents.h"
std::map<Texture::TextureType, std::string> UI::m_texLabels = {
    {Texture::TextureType::ALBEDO_MAP, "Albedo map"},
    {Texture::TextureType::METALLIC_MAP, "Metallic map"},
    {Texture::TextureType::ROUGHNESS_MAP, "Roughness map"},
    {Texture::TextureType::AO_MAP, "AO map"},
    {Texture::TextureType::OPACITY_MAP, "Opacity map"},
    {Texture::TextureType::CUBE_MAP, "Cube map"},
    {Texture::TextureType::NORMAL_MAP, "Normal map"},
};

UI::UI(): m_chosenAssetId(0), m_chosenAssetType(Asset::AssetType::TEXTURE), m_uiFlags(), m_activeNode(-1)
{
    //InputEvents::MouseButtonLeftPressedEvent::subscribe(std::bind(&UI::setNoActiveNode, this));

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

void UI::renderInterface(EntityUI* node, UIAssetMaps* uiAssets, GraphicsFlags* postprocFlags, const ComponentMapUI* componentMap)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //////////////////////
    //Create a main menu bar
    //////////////////////
    if (m_uiFlags.mainMenuVisible()) {
        if (ImGui::BeginMainMenuBar()) {

            if (ImGui::BeginMenu("Menus")) {
                bool assetVis = m_uiFlags.assetWindowVisible();
                bool sceneGraphVis = m_uiFlags.sceneGraphVisible();
                bool sceneNodeInspVis = m_uiFlags.sceneNodeInspectorVisible();
                if (ImGui::Checkbox("Asset Manager", &assetVis))
                    InputEvents::AssetWindowVisibilityEvent::notify(assetVis);
                if (ImGui::Checkbox("Scene Graph", &sceneGraphVis))
                    InputEvents::SceneGraphVisibilityEvent::notify(sceneGraphVis);
                if (ImGui::Checkbox("Scene Node Inspector", &sceneNodeInspVis))
                    InputEvents::SceneNodeInspectorVisibilityEvent::notify(sceneNodeInspVis);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Import")) {
                if (ImGui::BeginMenu("Texture")) {
                    if (ImGui::MenuItem("R")) {
                        InputEvents::importTextureREvent::notify();
                    }
                    if (ImGui::MenuItem("RGB")) {
                        InputEvents::importTextureRGBEvent::notify();
                    }
                    if (ImGui::MenuItem("sRGB")) {
                        InputEvents::importTextureSRGBEvent::notify();
                    }
                    
                    ImGui::EndMenu();

                }
                if (ImGui::MenuItem("3D model")) {
                    InputEvents::import3DModelEvent::notify();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Postprocessing")) {
                if (ImGui::Checkbox("Sharpen", &postprocFlags->sharpness))
                    InputEvents::PostprocSharpnessEvent::notify(postprocFlags->sharpness);
                if (ImGui::Checkbox("Grayscale", &postprocFlags->grayscale))
                    InputEvents::PostprocGrayscaleEvent::notify(postprocFlags->grayscale);
                if (ImGui::Checkbox("Negative", &postprocFlags->negative))
                    InputEvents::PostprocNegativeEvent::notify(postprocFlags->negative);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Directional light")) {
                ImGui::DragFloat3("Color", &((*postprocFlags->dirLightColor).x), 0.1f);
                ImGui::DragFloat3("Direction", &((*postprocFlags->dirLightDir).x), 0.1f);
                ImGui::DragFloat("Shadow bias", postprocFlags->shadowBias, 0.001f);
                ImGui::Checkbox("On", postprocFlags->dirLightActivated);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Anti-aliasing")) {
                if (ImGui::Checkbox("MSAA", &postprocFlags->MSAA))
                    InputEvents::MSAAToggleEvent::notify(postprocFlags->MSAA);
                ImGui::EndMenu();
            }
            bool tempBool = false;
            if (ImGui::BeginMenu("Skybox")) {
                ImGui::Checkbox("Clouds", &tempBool);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Shader")) {
                if (ImGui::Button("Phong"))
                    InputEvents::LightingBlinnEvent::notify(false);
                if (ImGui::Button("Blinn-Phong"))
                    InputEvents::LightingBlinnEvent::notify(true);
                if (ImGui::Button("Hot reload")) {
                    InputEvents::ShaderHotReloadEvent::notify(ShaderManager::ShaderType::PBR);
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Physics")) {
                if (ImGui::Button("Collider Visibility: off"))
                    InputEvents::ColliderVisibilityEvent::notify(false);
                if (ImGui::Button("Collider Visibility: on"))
                    InputEvents::ColliderVisibilityEvent::notify(true);

                ImGui::EndMenu();
            }
            if (ImGui::Button("Borderless/Bordered")) {
                InputEvents::WindowBordersToggleEvent::notify();
            }
            if (ImGui::BeginMenu("Debug")) {
                ImGui::Checkbox("Dear ImGui stack tool", &m_uiFlags.imguiStackToolVisible);
                ImGui::Checkbox("Dear ImGui Demo", &m_uiFlags.imguiDemoVisible);
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Play")) {
                if (ImGui::Button("On"))
                    InputEvents::PlayGameEvent::notify();
                if (ImGui::Button("Off"))
                    InputEvents::StopGameEvent::notify();

                ImGui::EndMenu();
            }
            if (ImGui::Button("Benchmark!")) {
                InputEvents::GPUBenchmarkEvent::notify();
            }
            ImGui::EndMainMenuBar();
            
        }
    }


    //////////////////////
    //Create a UI window for scenegraph
    //////////////////////
    if (m_uiFlags.sceneGraphVisible()) {
        ImGui::Begin("Scene");
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        if (ImGui::Button("Add node")) {
            InputEvents::AddNodeEvent::notify();
        }
        if (ImGui::Button("Parent to root")) {
            if (m_activeNode >= 0)
                InputEvents::SetNodeParentToRootEvent::notify(m_activeNode);
        }
        /*
        * Create a tree of scene nodes
        */
        ImGuiTreeNodeFlags treeflags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
        createSceneTree(node, treeflags);

        ImGui::PopStyleVar();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
    //////////////////////
    
    //////////////////////
    //Create a UI window for Node Inspector
    //////////////////////
    if (m_uiFlags.sceneNodeInspectorVisible()) {
        ImGui::Begin("Node Inspector");
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        if (m_activeNode == -1) {
            ImGui::Text("Choose a node from the scene graph.");
        }
        else
        {
            if (ImGui::Button("Add Lightsoure")) {
                InputEvents::AddLightComponentEvent::notify(m_activeNode);
            };
            if (ImGui::Button("Add 3D Model")) {
                InputEvents::Add3DModelComponentEvent::notify(m_activeNode);
            };
             if (ImGui::Button("Add RigidBody")) {
                InputEvents::AddRigidBodyComponentEvent::notify(m_activeNode, RigidBody::RigidBodyType::SBOX);
            };
              if (ImGui::Button("Add Camera")) {
                InputEvents::AddCameraEvent::notify(m_activeNode);
            };

            
            float sliderSpeed = 0.01f;
            for (auto& component : componentMap->at(m_activeNode)) {
                component->render(m_activeNode, *uiAssets);
            }
            if (ImGui::Button("Delete Node >:)")) {
                InputEvents::DeleteEntityEvent::notify(m_activeNode);
                InputEvents::SetActiveNodeEvent::notify(0);
                m_activeNode = -1;
            }
            
        }
        
        ImGui::PopStyleVar();
        ImGui::End();
    }
    //////////////////////
    
    //////////////////////
    //Create a UI window asset viewing
    //////////////////////
    if (m_uiFlags.assetWindowVisible()) {
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
                    std::string previewVal;
                    if (smesh.second == 0)
                        previewVal = "No material";
                    else
                        previewVal = uiAssets->materials.at(smesh.second).name;
                    if (ImGui::BeginCombo("##materialcombo", previewVal.c_str())) {
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
                if (ImGui::Button("Delete Material")) {
                    InputEvents::DeleteMaterialEvent::notify(chosenMat->id);
                    m_chosenAssetId = 0;
                }
                createMatTexCombos(chosenMat, uiAssets);
 
                /*
                * Material properties
                */
                for (auto& prop : *chosenMat->propertiesf) {
                    ImGui::Text(prop.first.c_str());
                    ImGui::DragFloat(prop.first.c_str(), &prop.second, 0.01);
                }
                for (auto& prop : *chosenMat->propertiesv3) {
                    ImGui::Text(prop.first.c_str());
                    ImGui::DragFloat3(prop.first.c_str(), &prop.second.r, 0.01);
                }
                for (auto& prop : *chosenMat->propertiesv4) {
                    ImGui::Text(prop.first.c_str());
                    ImGui::DragFloat4(prop.first.c_str(), &prop.second.r, 0.01);
                }

            }
        }

        ImGui::EndChild();

        ImGui::End();
    }
    
    //////////////////////

    //////////////////////
    //Show imgui debug window
    //////////////////////
    if (m_uiFlags.imguiStackToolVisible)
        ImGui::ShowStackToolWindow();
    //////////////////////
    
    //////////////////////
    // Dear Imgui demo window
    //////////////////////
    if (m_uiFlags.imguiDemoVisible)
        ImGui::ShowDemoWindow(&m_uiFlags.imguiDemoVisible);
    //////////////////////
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::setNoActiveNode()
{
    m_activeNode = -1;
}

void UI::sceneTreeRec(EntityUI* node, ImGuiTreeNodeFlags treeflags)
{
    ImGuiTreeNodeFlags nodeflags = treeflags;
    if (node->active)
        nodeflags |= ImGuiTreeNodeFlags_Selected;
    ImGui::PushID(node);
    bool node_open = ImGui::TreeNodeEx((void*)node->id, nodeflags, node->name.c_str());
    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
        InputEvents::SetActiveNodeEvent::notify(node->id);
        m_activeNode = node->id;
    }
    ImGui::PopID();
    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
    {
        // Set payload to carry the index of our item (could be anything)
        ImGui::SetDragDropPayload("SCENENODE", &node->id, sizeof(int));
        ImGui::Text("Reparent %s", node->name.c_str());
        ImGui::EndDragDropSource();
    }
    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENENODE"))
        {
            IM_ASSERT(payload->DataSize == sizeof(int));
            int payload_n = *(const int*)payload->Data;
            InputEvents::SetNodeParentEvent::notify(payload_n, node->id);
        }
        ImGui::EndDragDropTarget();
    }
    if (node_open)
    {
        for (auto child : node->children)
        {
            sceneTreeRec(&child, treeflags);
        }
        ImGui::TreePop();
    }
}

void UI::createSceneTree(EntityUI* rootNode, ImGuiTreeNodeFlags treeflags)
{
    for (auto child : rootNode->children)
    {
        sceneTreeRec(&child, treeflags);
    }
}

void UI::createMatTexCombos(MaterialUI* mat, UIAssetMaps* uiAssets)
{
    std::string comboLabel = "";
    for (auto const& tex : mat->textures) {
        if (tex.second == 0)
        {
            comboLabel = "No texture";
        }
        else
        {
            comboLabel = uiAssets->textures.at(tex.second).name;
        }
        if (ImGui::BeginCombo(m_texLabels.at(tex.first).c_str(), comboLabel.c_str())) {
            if (ImGui::Selectable("No texture", false)) {
                InputEvents::setMaterialTextureEvent::notify(mat->id, 0, tex.first);
            }
            for (auto const& asset : uiAssets->textures) {
                if (ImGui::Selectable(asset.second.name.c_str(), false)) {
                    InputEvents::setMaterialTextureEvent::notify(mat->id, asset.second.id, tex.first);
                }
            }
            ImGui::EndCombo();

        }
    }
}

void UI::mouseButtonReleaseHandler()
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(ImGuiMouseButton_Left , false);
}

//SceneNodeUI* UI::getNodeById(unsigned int id, SceneNodeUI* rootNode)
//{
//    if (rootNode->id == id)
//        return rootNode;
//    SceneNodeUI* retNode = nullptr;
//    SceneNodeUI* foundNode = nullptr;
//    for (auto child : rootNode->children) {
//        foundNode = getNodeById(id, &child);
//        if foundNode
//    }
//    return retNode;
//}

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

void UI::processNode(EntityUI* node, UIAssetMaps* uiAssets)
{
    //// Use object uid as identifier. Most commonly you could also use the object pointer as a base ID.
    //ImGui::PushID(node->uiElemId);

    //// Text and Tree nodes are less high than framed widgets, using AlignTextToFramePadding() we add vertical spacing to make the tree lines equal high.
    //ImGui::TableNextRow();
    //ImGui::TableSetColumnIndex(1);
    //if (ImGui::Button("Duplicate node")) {
    //    InputEvents::DuplicateNodeEvent::notify(node->id);
    //}
    //if (ImGui::Button("Set active")) {
    //    InputEvents::SetActiveNodeEvent::notify(node->id);
    //}

    //ImGui::TableSetColumnIndex(0);
    //ImGui::AlignTextToFramePadding();

    //if (ImGui::TreeNode("Object", "%s", node->name->c_str()))
    //{
    //    float sliderSpeed = 0.01f;
    //    // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
    //    //ImGui::TableNextRow();
    //    //ImGui::TableSetColumnIndex(0);
    //    //ImGui::AlignTextToFramePadding();
    //    //ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
    //    //ImGui::TreeNodeEx("Position", flags);
    //    //ImGui::TableSetColumnIndex(1);
    //    //ImGui::SetNextItemWidth(-FLT_MIN);
    //    //ImGui::DragFloat3("Position", &node->pos->x, sliderSpeed);

    //    //ImGui::TableNextRow();
    //    //ImGui::TableSetColumnIndex(0);
    //    //ImGui::AlignTextToFramePadding();
    //    //ImGui::TreeNodeEx("Scale", flags);
    //    //ImGui::TableSetColumnIndex(1);
    //    //ImGui::SetNextItemWidth(-FLT_MIN);
    //    //ImGui::DragFloat3("Scale", &node->scale->x, sliderSpeed);

    //    //ImGui::TableNextRow();
    //    //ImGui::TableSetColumnIndex(0);
    //    //ImGui::AlignTextToFramePadding();
    //    //ImGui::TreeNodeEx("Rotation", flags);
    //    //ImGui::TableSetColumnIndex(1);
    //    //ImGui::SetNextItemWidth(-FLT_MIN);
    //    //ImGui::DragFloat3("Rotation", &node->orientationEuler->x, sliderSpeed);

    //    //ImGui::TableNextRow();
    //    //ImGui::TableSetColumnIndex(0);
    //    //ImGui::AlignTextToFramePadding();
    //    //ImGui::TreeNodeEx("Mesh", flags);

    //    //ImGui::TableSetColumnIndex(1);
    //    //const char* meshboxlabel;
    //    //if (node->mesh != nullptr)
    //    //    meshboxlabel = node->mesh->name.c_str();
    //    //else
    //    //    meshboxlabel = "No Mesh";
    //    //if (ImGui::BeginCombo("##meshbox", meshboxlabel)) {
    //    //    if (ImGui::Selectable("No Mesh", false))
    //    //    {
    //    //        InputEvents::SetNodeMeshEvent::notify(node->id, 0);
    //    //    }
    //    //    for (auto const& mesh : uiAssets->meshes) {
    //    //        if (ImGui::Selectable(mesh.second.name.c_str(), false))
    //    //        {
    //    //            InputEvents::SetNodeMeshEvent::notify(node->id, mesh.second.id);
    //    //        }
    //    //    }
    //    //    ImGui::EndCombo();
    //    //}
    //    //// Wireframe mode checkbox
    //    ////ImGui::TableSetColumnIndex(2);
    //    //ImGui::Checkbox("Wireframe", node->wireframeMode);

    //    //// LightSource adding/removing
    //    //bool hasLight = node->hasLightsource;
    //    //if (ImGui::Checkbox("Light Source", &node->hasLightsource)){
    //    //    if (hasLight)
    //    //        InputEvents::SceneNodeLightsourceRemoveEvent::notify(node->id);
    //    //    else
    //    //        InputEvents::SceneNodeLightsourceAddEvent::notify(node->id);
    //    //}
    //
    //    for (auto child : node->children)
    //    {
    //        processNode(&child, uiAssets);
    //    }
    //    ImGui::TreePop();
    //}
    //    ImGui::PopID();
}
