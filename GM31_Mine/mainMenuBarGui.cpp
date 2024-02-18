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
	// 画像を読み込み
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
				// シーンをセーブ
				Manager::GetInstance()->SaveEditScene();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject")) {
			if (ImGui::BeginMenu("AddGameObject")) {
				if (ImGui::MenuItem("Enpty")) {
					// 立方体オブジェクトを追加
					Manager::GetInstance()->GetScene()->AddGameObject<GameObject>(1, "GameObject");
				}
				if (ImGui::MenuItem("Camera")) {
					// 立方体オブジェクトを追加
					Manager::GetInstance()->GetScene()->AddGameObject<CameraObject>(0, "Camera");
				}
				if (ImGui::MenuItem("Box")) {
					// 立方体オブジェクトを追加
					Manager::GetInstance()->GetScene()->AddGameObject<Box>(1, "Box");
				}
				if (ImGui::MenuItem("Player")) {
					// 球体オブジェクトを追加
					Manager::GetInstance()->GetScene()->AddGameObject<Humanoid>(1, "Humanoid");
				}
				if (ImGui::MenuItem("Field")) {
					// 球体オブジェクトを追加
					Manager::GetInstance()->GetScene()->AddGameObject<FieldObject>(1, "Field");
				}
				if (ImGui::MenuItem("SkyDome")) {
					// 球体オブジェクトを追加
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
		// 実行ボタン
		if (ImGui::ImageButton((void*)m_executeBtnTexture, ImVec2(16, 12))) {
			// 現在のエンジンの状態によって分岐
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

		// 一時停止ボタン
		if (ImGui::ImageButton((void*)m_stopBtnTexture, ImVec2(16, 12))) {
			switch (Manager::GetInstance()->GetMode()) {
			case ENGINE_MODE::RUN:
				// 実行中なら一時停止する
				Manager::GetInstance()->SetEngineMode(ENGINE_MODE::PAUSE);
				break;
			case ENGINE_MODE::PAUSE:
				// すでに一時停止しているなら再生
				Manager::GetInstance()->SetEngineMode(ENGINE_MODE::RUN);
				break;
			default:
				break;
			}
		}
		ImGui::EndMainMenuBar();
	}

}
