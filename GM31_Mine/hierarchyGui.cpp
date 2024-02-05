#include "MyImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "hierarchyGui.h"
#include "SceneGui.h"
#include "inspectorGui.h"

void HierarchyGui::Init()
{

}

void HierarchyGui::Update()
{

    ImGui::Begin("Hierarchy");
    // ツリーノードUIの設定フラグ（矢印をクリックでノード展開、選択状態の大きさ）
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow  | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selection_mask = (1 << 2);
    int node_clicked = -1;
    // シーンにある全てのゲームオブジェクト
    auto gameobjectList = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    int i = 0;
    // マウスで選択されたオブジェクト
    auto selectObj = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->GetSelectedObject();
    for (auto gameObject : gameobjectList)
    {
        // デフォルトの「シングルクリックで開く」のを無効にする
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
        // 子供がいる時のツリーノード
            //bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
            //if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            //    node_clicked = i;
            //if (node_open)
            //{
            //    ImGui::BulletText("Blah blah\nBlah Blah");
            //    ImGui::TreePop();
            //}
        // 子供がいないなら三角形がついていないノードにする
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObject->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            node_clicked = i;
            SetSelect(gameObject);
        }
        // マウスで選択されたオブジェクトがあるなら選択扱いにする
        if (selectObj && selectObj->GetID() == gameObject->GetID()) {
            node_clicked = i;
        }
        i++;
    }
    if (node_clicked != -1)
    {
        // Ctrlキーが押されていたなら範囲選択にする
        if (ImGui::GetIO().KeyCtrl)
            selection_mask ^= (1 << node_clicked);
        else 
            selection_mask = (1 << node_clicked); 
    }
	ImGui::End();
}

void HierarchyGui::SetSelect(GameObject* object)
{
    MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->SetSelectedObject(object);
    MyImGuiManager::GetInstance()->GetImGui<SceneGui>()->SetSelectedObject(object);
}
