#include "ImGui/imgui.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mainMenuBarGui.h"
#include "box.h"

void MainMenuBarGui::Init()
{
	// �摜��ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/ExecutionBtn.png",
		NULL,
		NULL,
		&m_executeBtnTexture,
		NULL);
	assert(m_executeBtnTexture);

	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"asset/texture/StopBtn.png",
		NULL,
		NULL,
		&m_stopBtnTexture,
		NULL);
	assert(m_stopBtnTexture);


}

void MainMenuBarGui::Update()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject")) {
			if (ImGui::BeginMenu("AddGameObject")) {
				if (ImGui::MenuItem("Box")) {
					// �����̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<Box>(1, "box01");
				}
				if (ImGui::MenuItem("Spherer")) {
					// ���̃I�u�W�F�N�g��ǉ�
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::SameLine(ImGui::GetWindowWidth()/2.0f);
		// ���s�{�^��
		if (ImGui::ImageButton((void*)m_executeBtnTexture, ImVec2(16, 12))) {
			// ���݂̃G���W���̏�Ԃɂ���ĕ���
			switch (Manager::GetInstance()->GetMode())
			{
				//case
				//	break;
				default:
					break;
			}
			// �Q�[���E�B���h�E�Ƀt�H�[�J�X��ݒ�
			// �G���W���̏�Ԃ����s��Ԃ�
		}
		// �ꎞ��~�{�^��
		if (ImGui::ImageButton((void*)m_stopBtnTexture, ImVec2(16, 12))) {

		}
		ImGui::EndMainMenuBar();
	}

}
