#include "myImGuiManager.h"
#include "main.h"
#include "scene.h"
#include "renderer.h"
#include "gameGui.h"

void GameGui::Init()
{
}

void GameGui::Update()
{
	ImGui::Begin("Game");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	ImVec2 imageSize = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)Renderer::GetGameShaderResourceView(), imageSize);
	ImGui::End();

}
