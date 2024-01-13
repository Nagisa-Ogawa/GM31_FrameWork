#include "MyImGuiManager.h"
#include "main.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "game.h"
#include "mainMenuBarGui.h"
#include "SceneGui.h"
#include "inspectorGui.h"
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

		ImGui::SameLine(ImGui::GetWindowWidth()/2.0f-100);
		switch (Manager::GetInstance()->GetMode())
		{
		case ENGINE_MODE::EDIT:
			ImGui::Text("MODE : EDIT");
			break;
		case ENGINE_MODE::RUN:
			ImGui::Text("MODE : RUN");
			break;
		case ENGINE_MODE::PAUSE:
			ImGui::Text("MODE : PAUSE");
			break;
		default:
			break;
		}
		// ���s�{�^��
		if (ImGui::ImageButton((void*)m_executeBtnTexture, ImVec2(16, 12))) {
			// ���݂̃G���W���̏�Ԃɂ���ĕ���
			switch (Manager::GetInstance()->GetMode())
			{
			case ENGINE_MODE::EDIT:
				// �Q�[���E�B���h�E�Ƀt�H�[�J�X��ݒ�
				ImGui::SetWindowFocus("Game");
				// ���s��Ԃ�
				Manager::GetInstance()->SetEngineMode(ENGINE_MODE::RUN);
				// �X�N���v�g��Start�֐����Ăяo��
				Manager::GetInstance()->GetScene()->CallScriptStartFunc();
				break;
			case ENGINE_MODE::RUN:
			{
				// �G�f�B�^�E�B���h�E�Ƀt�H�[�J�X��ݒ�
				ImGui::SetWindowFocus("Scene");
				// �G�f�B�^�ŃI�u�W�F�N�g��I�����Ă���ꍇ��x���Z�b�g
				auto sceneGui = MyImGuiManager::GetInstance()->GetImGui<SceneGui>();
				auto inspectorGui = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
				sceneGui->SetSelectedObject(NULL);
				inspectorGui->SetSelectedObject(NULL);
				// �V�[������蒼���ă��Z�b�g
				Manager::GetInstance()->SetScene<Game>();
				// ���s�I��
				Manager::GetInstance()->SetEngineMode(ENGINE_MODE::EDIT);
				break;
			}
			default:
				break;
			}
		}

		// �ꎞ��~�{�^��
		if (ImGui::ImageButton((void*)m_stopBtnTexture, ImVec2(16, 12))) {
			switch (Manager::GetInstance()->GetMode()) {
			case ENGINE_MODE::RUN:
				// ���s���Ȃ�ꎞ��~����
				Manager::GetInstance()->SetEngineMode(ENGINE_MODE::PAUSE);
				break;
			case ENGINE_MODE::PAUSE:
				// ���łɈꎞ��~���Ă���Ȃ�Đ�
				Manager::GetInstance()->SetEngineMode(ENGINE_MODE::RUN);
				break;
			default:
				break;
			}
		}
		ImGui::EndMainMenuBar();
	}

}
