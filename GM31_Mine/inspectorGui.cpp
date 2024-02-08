#include "MyImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "boxCollision.h"
#include "camera.h"
#include "model.h"
#include "quadCollision.h"
#include "script.h"
#include "shader.h"
#include "sphereCollision.h"
#include "sprite.h"
#include "transform.h"
#include "MyImGuiManager.h"
#include "dispInspector.h"
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
		ImGui::Separator();
		// �A�N�e�B�u�t���OGUI
		bool isActive = m_selectedObject->GetActive();
		if (ImGui::Checkbox("##active", &isActive)) {
			m_selectedObject->SetActive(isActive);
		}
		ImGui::SameLine();
		// �I�u�W�F�N�g��GUI
		char str[128];
		strcpy_s(str, m_selectedObject->GetName().c_str());
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("##name", str, sizeof(str), ImGuiInputTextFlags_EnterReturnsTrue)) {
			std::string s(str);
			m_selectedObject->SetName(s);
		}
		ImGui::PopItemWidth();
		ImGui::Separator();
		// �I������Ă���I�u�W�F�N�g�ɃR���|�[�l���g����Ă���N���X���擾
		auto componentList = m_selectedObject->GetAllComponent();
		// �R���|�[�l���g�̃N���X�ɉ����ăC���X�y�N�^�[��\������֐����Ă�
		for (auto component : componentList) {
			component->DispInspector();
		}
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2.0f - 50.0f);
		// �R���|�[�l���g�ǉ��{�^��
		if (ImGui::Button("AddComponent")) {
			m_isAddComp = !m_isAddComp;
		}
		if (m_isAddComp) {
			const char* compNames[] = { "BoxCollision", "Camera", "Model", "QuadCollision", "Script", "Shader", "SphereCollision"};
			int item_current_idx = -1; // Here we store our selection data as an index.
			if (ImGui::BeginListBox("##addComponentList"))
			{
				for (int n = 0; n < IM_ARRAYSIZE(compNames); n++)
				{
					const bool is_selected = (item_current_idx == n);
					if (ImGui::Selectable(compNames[n], is_selected)) {
						item_current_idx = n;
					}
				}
				ImGui::EndListBox();
			}
			if (item_current_idx != -1) {
				switch (item_current_idx)
				{
				case 0:
					m_selectedObject->AddComponent<BoxCollision>();
					break;
				case 1:
					m_selectedObject->AddComponent<Camera>();
					break;
				case 2:
					m_selectedObject->AddComponent<Model>();
					break;
				case 3:
					m_selectedObject->AddComponent<QuadCollision>();
					break;
				case 4:
					m_selectedObject->AddComponent<Script>();
					break;
				case 5:
					m_selectedObject->AddComponent<Shader>();
					break;
				case 6:
					m_selectedObject->AddComponent<SphereCollision>();
					break;
				default:
					break;
				}
				m_isAddComp = false;
			}
		}
	}
	ImGui::End();

}
