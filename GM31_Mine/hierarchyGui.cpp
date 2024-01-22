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
    //MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
    //// �I�u�W�F�N�g�̐��������[�v
    //for (auto i = m_gameObjecMap.begin(); i != m_gameObjecMap.end(); ++i)
    //{
    //    //char buf[32];
    //    //sprintf(buf, "Object %d", n);
    //    if (ImGui::Selectable(i->first.c_str(), i->second))
    //    {
    //        for (auto j = m_gameObjecMap.begin(); j != m_gameObjecMap.end(); ++j)
    //        {
    //            j->second = false;
    //        }
    //        // �_�u���N���b�N���̏���
    //        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
    //            // �J�������N���b�N���ꂽ�I�u�W�F�N�g��

    //        }
    //        // Ctrl�L�[�������Ȃ���ŕ����I�����\�ɂ���
    //            //if (!ImGui::GetIO().KeyCtrl)
    //            //    memset(selection, 0, sizeof(selection));
    //        i->second ^= true;
    //    }
    //}
    // �c���[�m�[�hUI�̐ݒ�t���O�i�����N���b�N�Ńm�[�h�W�J�A�I����Ԃ̑傫���j
    static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow  | ImGuiTreeNodeFlags_SpanAvailWidth;
    static int selection_mask = (1 << 2);
    int node_clicked = -1;
    auto gameobjectList = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    int i = 0;
    for (auto gameObject : gameobjectList)
    {
        // �f�t�H���g�́u�V���O���N���b�N�ŊJ���v�̂𖳌��ɂ���
        ImGuiTreeNodeFlags node_flags = base_flags;
        const bool is_selected = (selection_mask & (1 << i)) != 0;
        if (is_selected)
            node_flags |= ImGuiTreeNodeFlags_Selected;
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
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObject->GetName().c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            node_clicked = i;
            SetSelect(gameObject);
        }
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
            // �J�������N���b�N���ꂽ�I�u�W�F�N�g��
            SetSelect(gameObject);
        }
        i++;
    }
    if (node_clicked != -1)
    {
        // Ctrl�L�[��������Ă����Ȃ�͈͑I���ɂ���
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
