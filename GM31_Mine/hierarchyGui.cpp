#include "myImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "hierarchyGui.h"
#include "sceneGui.h"
#include "inspectorGui.h"
#include "objectTreeNode.h"

void HierarchyGui::Init()
{
}

void HierarchyGui::Update()
{
    ImGui::Begin("Hierarchy");
    // シーンのオブジェクトを木構造で表示
    ShowObjectTree();
	ImGui::End();
}


/// <summary>
/// シーンのオブジェクトを木構造で表示する関数
/// </summary>
void HierarchyGui::ShowObjectTree()
{
    // 親のいないオブジェクト（一番親）をリストで取得
    auto objectList = m_sceneObjectTree->GetChild();
    // マウスで選択されたオブジェクト
    auto selectObj = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->GetSelectedObject();
    // ヒエラルキーウィンドウ以外からオブジェクトを選択していた場合はヒエラルキーウィンドウでも選択状態にする
    if (selectObj && m_selectionID != selectObj->GetID()) {
        m_selectionID = selectObj->GetID();
    }
    // 一番親のオブジェクトから木構造でオブジェクトを表示
    for (auto objectNode : objectList) {
        ShowObjectNode(objectNode, &m_selectionID);
    }
    // 選択されたノードがあるかチェック
    if (m_selectionID != -1)
    {
        m_selectionMask = (1 << m_selectionID);
    }

}


/// <summary>
/// オブジェクトツリーノードを表示する関数
/// </summary>
/// <param name="objectNode">表示するオブジェクト</param>
/// <param name="selectionID"></param>
void HierarchyGui::ShowObjectNode(ObjectTreeNode* objectNode, int* selectionID)
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    // このノードが選択されているなら選択フラグをONにする
    const bool isSelected = (m_selectionMask & (1 << objectNode->GetID())) != 0;
    if (isSelected)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    // このオブジェクトに子供なら子供がいる時用のノードにする
    auto childList = objectNode->GetChild();
    if (childList.size() > 0) {
        // 子供がいる時のツリーノード
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)objectNode->GetID(), nodeFlags, objectNode->GetTreeObject()->GetName().c_str(), objectNode->GetID());
        // ノードがクリックされたなら選択状態にする
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectionID = objectNode->GetID();
            SetSelect(objectNode->GetTreeObject());
        }
        // ノードが展開されているなら
        if (isNodeOpen)
        {
            // このノードの子ノードかつフォルダであるノードを表示
            auto dirList = objectNode->GetChild();
            for (auto child : dirList) {
                ShowObjectNode(child, selectionID);
            }
            ImGui::TreePop();
        }
    }
    else {
        // 子供がいないなら三角形がついていないノードにする
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)objectNode->GetID(), nodeFlags, objectNode->GetTreeObject()->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectionID = objectNode->GetID();
            SetSelect(objectNode->GetTreeObject());
        }
    }
}


/// <summary>
/// オブジェクトツリーに新しいオブジェクトを追加する関数
/// </summary>
/// <param name="object">追加するオブジェクト</param>
void HierarchyGui::AddObjectNode(GameObject* object)
{
    if (m_sceneObjectTree) {
        m_sceneObjectTree->Insert(object);
    }
}


/// <summary>
/// オブジェクトの親ノードを変更する関数
/// </summary>
/// <param name="nodeID">自分のID</param>
/// <param name="parentID">元の親のID</param>
/// <param name="nextParentID">変更先の親のID</param>
void HierarchyGui::ChangeParentNode(int nodeID, int parentID, int nextParentID)
{
    // IDからそれぞれのノードを検索
    ObjectTreeNode* objectNode = m_sceneObjectTree->FindNode(nodeID);
    ObjectTreeNode* parentNode = m_sceneObjectTree->FindNode(parentID);
    ObjectTreeNode* nextParentNode=m_sceneObjectTree->FindNode(nextParentID);
    // 元の親から自分を削除
    if (parentNode) {
        parentNode->DeleteChild(objectNode->GetID());
    }
    // 新しい親に追加
    if (nextParentNode) {
        nextParentNode->InsertChild(objectNode);
    }
}


/// <summary>
/// オブジェクトノードを削除する関数
/// </summary>
/// <param name="objectID">削除するノードのID</param>
void HierarchyGui::DeleteObjectNode(int objectID)
{
    // IDから削除するノードを検索
    ObjectTreeNode* objectNode = m_sceneObjectTree->FindNode(objectID);
    // ノードがあったなら子供ごと削除
    if (objectNode) {
        // 親の子供リストから自分を削除
        if (objectNode->GetParent()) {
            objectNode->GetParent()->DeleteChild(objectID);
        }
        objectNode->Delete(true);
    }
}


/// <summary>
/// シーンのオブジェクトツリーを初期化する関数
/// </summary>
/// <param name="sceneTreeNode">シーンのオブジェクトツリー</param>
void HierarchyGui::InitObjectTree()
{
    m_sceneObjectTree = new ObjectTreeNode();
    m_sceneObjectTree->SetID(-2);
    // 親のいないオブジェクトを探す
    auto objectList = Manager::GetInstance()->GetScene()->GetMostParentObjects();
    // 親のいないオブジェクトを全てツリーノードに格納
    for (auto object : objectList) {
        auto objectNode = m_sceneObjectTree->Insert(object);
        CreateObjectNode(objectNode);
    }
}


/// <summary>
/// オブジェクトをツリーノードに格納する関数
/// </summary>
/// <param name="objectNode">格納するツリーノード</param>
void HierarchyGui::CreateObjectNode(ObjectTreeNode* objectNode)
{
    auto transform = objectNode->GetTreeObject()->GetTransform();
    // 現在のノードの子供を取得
    auto childObjectList = transform->GetChildList();
    for (auto child : childObjectList) {
        // 子供のノードを格納
        auto childNode = objectNode->Insert(child->GetGameObject());
        // ツリーノードに格納
        CreateObjectNode(childNode);
    }
}

void HierarchyGui::SetSelect(GameObject* object)
{
    MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->SetSelectedObject(object);
    MyImGuiManager::GetInstance()->GetImGui<SceneGui>()->SetSelectedObject(object);
}
