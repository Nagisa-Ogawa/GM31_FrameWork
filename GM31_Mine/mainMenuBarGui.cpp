#include "ImGui/imgui.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "mainMenuBarGui.h"
#include "box.h"

void MainMenuBarGui::Init()
{
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
		ImGui::EndMainMenuBar();
	}

}
