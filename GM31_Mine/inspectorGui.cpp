#include "MyImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
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
		// �I������Ă���I�u�W�F�N�g�ɃR���|�[�l���g����Ă���N���X���擾
		auto componentList = m_selectedObject->GetAllComponent();
		// �R���|�[�l���g�̃N���X�ɉ����ăC���X�y�N�^�[��\������֐����Ă�
		for (auto component : componentList) {
			component->DispInspector();
		}
	}
	//if (m_selectedObject) {

	//}
	ImGui::End();

}
