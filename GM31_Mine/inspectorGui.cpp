#include "ImGui/imgui.h"
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
	if (m_forcusObject) {
		ImGui::Text("ObjectType : %s", typeid(*m_forcusObject).name());
		if (ImGui::TreeNode("Position")) {
			ImGui::Text("x:%.3f y:%.3f z:%.3f",
				m_forcusObject->GetTransform()->m_position.x, m_forcusObject->GetTransform()->m_position.y, m_forcusObject->GetTransform()->m_position.z);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Rotation")) {
			ImGui::Text("x:%.3f y:%.3f z:%.3f",
				m_forcusObject->GetTransform()->m_rotation.x, m_forcusObject->GetTransform()->m_rotation.y, m_forcusObject->GetTransform()->m_rotation.z);
			ImGui::TreePop();
		}
	}
	ImGui::End();

}
