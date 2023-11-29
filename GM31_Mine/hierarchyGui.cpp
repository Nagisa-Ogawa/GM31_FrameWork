#include "ImGui/imgui.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "hierarchyGui.h"

void HierarchyGui::Init()
{
    auto gameObjects = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    // test�p �����ō�����I�u�W�F�N�g���擾
    for (GameObject* gameObject : gameObjects)
    {
        m_gameObjecMap[gameObject->GetName()] = false;
    }

}

void HierarchyGui::Update()
{
    auto gameObjects = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    // test�p �����ō�����I�u�W�F�N�g���擾
    for (GameObject* gameObject : gameObjects)
    {
        // �V�����ł����I�u�W�F�N�g������Ȃ�ǉ�
        if (!m_gameObjecMap[gameObject->GetName()]) {
            m_gameObjecMap[gameObject->GetName()] = false;
        }
    }

    ImGui::Begin("Hierarchy");
    // �I�u�W�F�N�g�̐��������[�v
    for (auto i = m_gameObjecMap.begin(); i != m_gameObjecMap.end(); ++i)
    {
        //char buf[32];
        //sprintf(buf, "Object %d", n);
        if (ImGui::Selectable(i->first.c_str(), i->second))
        {
            for (auto j = m_gameObjecMap.begin(); j != m_gameObjecMap.end(); ++j)
            {
                j->second = false;
            }
            // �_�u���N���b�N���̏���
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                // �J�������N���b�N���ꂽ�I�u�W�F�N�g��

            }
            // Ctrl�L�[�������Ȃ���ŕ����I�����\�ɂ���
                //if (!ImGui::GetIO().KeyCtrl)
                //    memset(selection, 0, sizeof(selection));
            i->second ^= true;
        }
    }
	ImGui::End();
}
