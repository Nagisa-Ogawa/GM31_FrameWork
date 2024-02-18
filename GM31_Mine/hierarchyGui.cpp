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
    // �V�[���̃I�u�W�F�N�g��؍\���ŕ\��
    ShowObjectTree();
	ImGui::End();
}


/// <summary>
/// �V�[���̃I�u�W�F�N�g��؍\���ŕ\������֐�
/// </summary>
void HierarchyGui::ShowObjectTree()
{
    // �e�̂��Ȃ��I�u�W�F�N�g�i��Ԑe�j�����X�g�Ŏ擾
    auto objectList = m_sceneObjectTree->GetChild();
    // �}�E�X�őI�����ꂽ�I�u�W�F�N�g
    auto selectObj = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->GetSelectedObject();
    // �q�G�����L�[�E�B���h�E�ȊO����I�u�W�F�N�g��I�����Ă����ꍇ�̓q�G�����L�[�E�B���h�E�ł��I����Ԃɂ���
    if (selectObj && m_selectionID != selectObj->GetID()) {
        m_selectionID = selectObj->GetID();
    }
    // ��Ԑe�̃I�u�W�F�N�g����؍\���ŃI�u�W�F�N�g��\��
    for (auto objectNode : objectList) {
        ShowObjectNode(objectNode, &m_selectionID);
    }
    // �I�����ꂽ�m�[�h�����邩�`�F�b�N
    if (m_selectionID != -1)
    {
        m_selectionMask = (1 << m_selectionID);
    }

}


/// <summary>
/// �I�u�W�F�N�g�c���[�m�[�h��\������֐�
/// </summary>
/// <param name="objectNode">�\������I�u�W�F�N�g</param>
/// <param name="selectionID"></param>
void HierarchyGui::ShowObjectNode(ObjectTreeNode* objectNode, int* selectionID)
{
    ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
    // ���̃m�[�h���I������Ă���Ȃ�I���t���O��ON�ɂ���
    const bool isSelected = (m_selectionMask & (1 << objectNode->GetID())) != 0;
    if (isSelected)
        nodeFlags |= ImGuiTreeNodeFlags_Selected;
    // ���̃I�u�W�F�N�g�Ɏq���Ȃ�q�������鎞�p�̃m�[�h�ɂ���
    auto childList = objectNode->GetChild();
    if (childList.size() > 0) {
        // �q�������鎞�̃c���[�m�[�h
        nodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        bool isNodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)objectNode->GetID(), nodeFlags, objectNode->GetTreeObject()->GetName().c_str(), objectNode->GetID());
        // �m�[�h���N���b�N���ꂽ�Ȃ�I����Ԃɂ���
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectionID = objectNode->GetID();
            SetSelect(objectNode->GetTreeObject());
        }
        // �m�[�h���W�J����Ă���Ȃ�
        if (isNodeOpen)
        {
            // ���̃m�[�h�̎q�m�[�h���t�H���_�ł���m�[�h��\��
            auto dirList = objectNode->GetChild();
            for (auto child : dirList) {
                ShowObjectNode(child, selectionID);
            }
            ImGui::TreePop();
        }
    }
    else {
        // �q�������Ȃ��Ȃ�O�p�`�����Ă��Ȃ��m�[�h�ɂ���
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)objectNode->GetID(), nodeFlags, objectNode->GetTreeObject()->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            *selectionID = objectNode->GetID();
            SetSelect(objectNode->GetTreeObject());
        }
    }
}


/// <summary>
/// �I�u�W�F�N�g�c���[�ɐV�����I�u�W�F�N�g��ǉ�����֐�
/// </summary>
/// <param name="object">�ǉ�����I�u�W�F�N�g</param>
void HierarchyGui::AddObjectNode(GameObject* object)
{
    if (m_sceneObjectTree) {
        m_sceneObjectTree->Insert(object);
    }
}


/// <summary>
/// �I�u�W�F�N�g�̐e�m�[�h��ύX����֐�
/// </summary>
/// <param name="nodeID">������ID</param>
/// <param name="parentID">���̐e��ID</param>
/// <param name="nextParentID">�ύX��̐e��ID</param>
void HierarchyGui::ChangeParentNode(int nodeID, int parentID, int nextParentID)
{
    // ID���炻�ꂼ��̃m�[�h������
    ObjectTreeNode* objectNode = m_sceneObjectTree->FindNode(nodeID);
    ObjectTreeNode* parentNode = m_sceneObjectTree->FindNode(parentID);
    ObjectTreeNode* nextParentNode=m_sceneObjectTree->FindNode(nextParentID);
    // ���̐e���玩�����폜
    if (parentNode) {
        parentNode->DeleteChild(objectNode->GetID());
    }
    // �V�����e�ɒǉ�
    if (nextParentNode) {
        nextParentNode->InsertChild(objectNode);
    }
}


/// <summary>
/// �I�u�W�F�N�g�m�[�h���폜����֐�
/// </summary>
/// <param name="objectID">�폜����m�[�h��ID</param>
void HierarchyGui::DeleteObjectNode(int objectID)
{
    // ID����폜����m�[�h������
    ObjectTreeNode* objectNode = m_sceneObjectTree->FindNode(objectID);
    // �m�[�h���������Ȃ�q�����ƍ폜
    if (objectNode) {
        // �e�̎q�����X�g���玩�����폜
        if (objectNode->GetParent()) {
            objectNode->GetParent()->DeleteChild(objectID);
        }
        objectNode->Delete(true);
    }
}


/// <summary>
/// �V�[���̃I�u�W�F�N�g�c���[������������֐�
/// </summary>
/// <param name="sceneTreeNode">�V�[���̃I�u�W�F�N�g�c���[</param>
void HierarchyGui::InitObjectTree()
{
    m_sceneObjectTree = new ObjectTreeNode();
    m_sceneObjectTree->SetID(-2);
    // �e�̂��Ȃ��I�u�W�F�N�g��T��
    auto objectList = Manager::GetInstance()->GetScene()->GetMostParentObjects();
    // �e�̂��Ȃ��I�u�W�F�N�g��S�ăc���[�m�[�h�Ɋi�[
    for (auto object : objectList) {
        auto objectNode = m_sceneObjectTree->Insert(object);
        CreateObjectNode(objectNode);
    }
}


/// <summary>
/// �I�u�W�F�N�g���c���[�m�[�h�Ɋi�[����֐�
/// </summary>
/// <param name="objectNode">�i�[����c���[�m�[�h</param>
void HierarchyGui::CreateObjectNode(ObjectTreeNode* objectNode)
{
    auto transform = objectNode->GetTreeObject()->GetTransform();
    // ���݂̃m�[�h�̎q�����擾
    auto childObjectList = transform->GetChildList();
    for (auto child : childObjectList) {
        // �q���̃m�[�h���i�[
        auto childNode = objectNode->Insert(child->GetGameObject());
        // �c���[�m�[�h�Ɋi�[
        CreateObjectNode(childNode);
    }
}

void HierarchyGui::SetSelect(GameObject* object)
{
    MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->SetSelectedObject(object);
    MyImGuiManager::GetInstance()->GetImGui<SceneGui>()->SetSelectedObject(object);
}
