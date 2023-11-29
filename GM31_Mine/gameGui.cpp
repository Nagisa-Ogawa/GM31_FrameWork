#include "ImGui/imgui.h"
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
	ImVec2 imageSize = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)Renderer::GetGameShaderResourceView(), imageSize);
	ImGui::End();

}
