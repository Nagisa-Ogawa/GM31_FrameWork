#include "MyImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "MyImGuiManager.h"
#include "inspectorGui.h"
#include "SceneGui.h"

void InspectorGui::Init()
{
}

void InspectorGui::Update()
{
	
	ImGui::Begin("Inspecter");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	if (m_selectedObject) {
		ImGui::Text("ObjectType : %s", typeid(*m_selectedObject).name());
		ImGui::SeparatorText("Transform");
		if (ImGui::TreeNodeEx("Position",ImGuiTreeNodeFlags_DefaultOpen)) {
			float pos[3] = { m_selectedObject->GetTransform()->m_position.x,m_selectedObject->GetTransform()->m_position.y ,m_selectedObject->GetTransform()->m_position.z };
			if (ImGui::InputFloat3("", pos)) {
				m_selectedObject->GetTransform()->m_position = D3DXVECTOR3(pos[0], pos[1], pos[2]);
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Rotation",ImGuiTreeNodeFlags_DefaultOpen)) {
			float rot[3] = { m_selectedObject->GetTransform()->GetRotationAsDegree().x,m_selectedObject->GetTransform()->GetRotationAsDegree().y ,m_selectedObject->GetTransform()->GetRotationAsDegree().z};
			if (ImGui::InputFloat3("", rot)) {
				m_selectedObject->GetTransform()->SetRotationFromDegree(D3DXVECTOR3(rot[0], rot[1], rot[2]));
			}
			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("Scale", ImGuiTreeNodeFlags_DefaultOpen)) {
			float scale[3] = { m_selectedObject->GetTransform()->m_scale.x,m_selectedObject->GetTransform()->m_scale.y ,m_selectedObject->GetTransform()->m_scale.z };
			if (ImGui::InputFloat3("", scale)) {
				m_selectedObject->GetTransform()->m_scale = D3DXVECTOR3(scale[0], scale[1], scale[2]);
			}
			ImGui::TreePop();
		}

	}
	ImGui::End();

}
