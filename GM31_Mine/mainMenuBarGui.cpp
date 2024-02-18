#include "MyImGuiManager.h"
#include "main.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mainMenuBarGui.h"
#include "SceneGui.h"
#include "inspectorGui.h"
#include "cameraObject.h"
#include "box.h"
#include "player.h"
#include "fieldObject.h"
#include "sky.h"

void MainMenuBarGui::Init()
{
	// �摜��ǂݍ���
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"Assets\\Textures\\ExecutionBtn.png",
		NULL,
		NULL,
		&m_executeBtnTexture,
		NULL);
	assert(m_executeBtnTexture);

	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		"Assets\\Textures\\StopBtn.png",
		NULL,
		NULL,
		&m_stopBtnTexture,
		NULL);
	assert(m_stopBtnTexture);


}

void MainMenuBarGui::Uninit()
{
	m_executeBtnTexture->Release();
	m_stopBtnTexture->Release();
}

void MainMenuBarGui::Update()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("SaveScene")) {
				// �V�[�����Z�[�u
				Manager::GetInstance()->SaveEditScene();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject")) {
			if (ImGui::BeginMenu("AddGameObject")) {
				if (ImGui::MenuItem("Enpty")) {
					// �����̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<GameObject>(1, "GameObject");
				}
				if (ImGui::MenuItem("Camera")) {
					// �����̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<CameraObject>(0, "Camera");
				}
				if (ImGui::MenuItem("Box")) {
					// �����̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<Box>(1, "Box");
				}
				if (ImGui::MenuItem("Player")) {
					// ���̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<Humanoid>(1, "Humanoid");
				}
				if (ImGui::MenuItem("Field")) {
					// ���̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<FieldObject>(1, "Field");
				}
				if (ImGui::MenuItem("SkyDome")) {
					// ���̃I�u�W�F�N�g��ǉ�
					Manager::GetInstance()->GetScene()->AddGameObject<Sky>(1, "Sky");
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
				Manager::GetInstance()->PlayScene();
				break;
			case ENGINE_MODE::RUN:
				Manager::GetInstance()->StopScene();
				break;
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
