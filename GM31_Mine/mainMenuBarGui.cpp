#include "ImGui/imgui.h"
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"
#include "mainMenuBarGui.h"
#include "box.h"

void MainMenuBarGui::Init()
{
	// 画像を読み込み
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
					// 立方体オブジェクトを追加
					Manager::GetInstance()->GetScene()->AddGameObject<Box>(1, "box01");
				}
				if (ImGui::MenuItem("Spherer")) {
					// 球体オブジェクトを追加
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::SameLine(ImGui::GetWindowWidth()/2.0f);
		// 実行ボタン
		if (ImGui::ImageButton((void*)m_executeBtnTexture, ImVec2(16, 12))) {
			// 現在のエンジンの状態によって分岐
			switch (Manager::GetInstance()->GetMode())
			{
				//case
				//	break;
				default:
					break;
			}
			// ゲームウィンドウにフォーカスを設定
			// エンジンの状態を実行状態へ
		}
		// 一時停止ボタン
		if (ImGui::ImageButton((void*)m_stopBtnTexture, ImVec2(16, 12))) {

		}
		ImGui::EndMainMenuBar();
	}

}
