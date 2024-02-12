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
    static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow  | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selectionMask = (1 << 2);
    int nodeClicked = -1;
    // シーンにある全てのゲームオブジェクト
    auto gameobjectList = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    int i = 0;
    // マウスで選択されたオブジェクト
    auto selectObj = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->GetSelectedObject();
    for (auto gameObject : gameobjectList)
    {
        // デフォルトの「シングルクリックで開く」のを無効にする
        ImGuiTreeNodeFlags nodeFlags = baseFlags;
        const bool isSelected = (selectionMask & (1 << i)) != 0;
        if (isSelected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
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
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, gameObject->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            nodeClicked = i;
            SetSelect(gameObject);
        }
        // マウスで選択されたオブジェクトがあるなら選択扱いにする
        if (selectObj && selectObj->GetID() == gameObject->GetID()) {
            nodeClicked = i;
        }
        i++;
    }
    // 選ばれたノードを選択状態にする
    if (nodeClicked != -1)
    {
        selectionMask = (1 << nodeClicked); 
    }
	ImGui::End();
}

void HierarchyGui::SetSelect(GameObject* object)
{
    MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->SetSelectedObject(object);
    MyImGuiManager::GetInstance()->GetImGui<SceneGui>()->SetSelectedObject(object);
}
