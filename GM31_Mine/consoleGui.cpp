#include "MyImGuiManager.h"
#include "consoleGui.h"

void ConsoleGui::Init()
{
	m_message = "test";
}

void ConsoleGui::Update()
{
	ImGui::Begin("Console");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	// �f�o�b�N�p�̏�񂪂���Ȃ�\��
	if (m_message.size() > 0) {
		ImGui::Text(m_message.c_str());
	}
	ImGui::End();

}
