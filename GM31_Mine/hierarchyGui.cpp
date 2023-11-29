#include "ImGui/imgui.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "hierarchyGui.h"

void HierarchyGui::Init()
{
    auto gameObjects = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    // test用 ここで今あるオブジェクトを取得
    for (GameObject* gameObject : gameObjects)
    {
        m_gameObjecMap[gameObject->GetName()] = false;
    }

}

void HierarchyGui::Update()
{
    auto gameObjects = Manager::GetInstance()->GetScene()->GetAllGameObjects();
    // test用 ここで今あるオブジェクトを取得
    for (GameObject* gameObject : gameObjects)
    {
        // 新しくできたオブジェクトがあるなら追加
        if (!m_gameObjecMap[gameObject->GetName()]) {
            m_gameObjecMap[gameObject->GetName()] = false;
        }
    }

    ImGui::Begin("Hierarchy");
    // オブジェクトの数だけループ
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
            // ダブルクリック時の処理
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
                // カメラをクリックされたオブジェクトへ

            }
            // Ctrlキーを押しながらで複数選択を可能にする
                //if (!ImGui::GetIO().KeyCtrl)
                //    memset(selection, 0, sizeof(selection));
            i->second ^= true;
        }
    }
	ImGui::End();
}
