#include "MyImGuiManager.h"
#include "projectGui.h"

void ProjectGui::Init()
{
}

void ProjectGui::Update()
{
	ImGui::Begin("Project");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	ImGui::End();

}
