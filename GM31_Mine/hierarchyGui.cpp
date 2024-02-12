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
    // �c���[�m�[�hUI�̐ݒ�t���O�i�����N���b�N�Ńm�[�h�W�J�A�I����Ԃ̑傫���j
    static ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow  | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selectionMask = (1 << 2);
    int nodeClicked = -1;
    // �V�[���ɂ���S�ẴQ�[���I�u�W�F�N�g
    auto gameobjectList = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    int i = 0;
    // �}�E�X�őI�����ꂽ�I�u�W�F�N�g
    auto selectObj = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>()->GetSelectedObject();
    for (auto gameObject : gameobjectList)
    {
        // �f�t�H���g�́u�V���O���N���b�N�ŊJ���v�̂𖳌��ɂ���
        ImGuiTreeNodeFlags nodeFlags = baseFlags;
        const bool isSelected = (selectionMask & (1 << i)) != 0;
        if (isSelected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        // �q�������鎞�̃c���[�m�[�h
            //bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
            //if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
            //    node_clicked = i;
            //if (node_open)
            //{
            //    ImGui::BulletText("Blah blah\nBlah Blah");
            //    ImGui::TreePop();
            //}
        // �q�������Ȃ��Ȃ�O�p�`�����Ă��Ȃ��m�[�h�ɂ���
        nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)i, nodeFlags, gameObject->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            nodeClicked = i;
            SetSelect(gameObject);
        }
        // �}�E�X�őI�����ꂽ�I�u�W�F�N�g������Ȃ�I�������ɂ���
        if (selectObj && selectObj->GetID() == gameObject->GetID()) {
            nodeClicked = i;
        }
        i++;
    }
    // �I�΂ꂽ�m�[�h��I����Ԃɂ���
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
