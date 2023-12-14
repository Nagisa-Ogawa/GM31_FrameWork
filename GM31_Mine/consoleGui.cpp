#include "MyImGuiManager.h"
#include "consoleGui.h"

void ConsoleGui::Init()
{
}

void ConsoleGui::Update()
{
	ImGui::Begin("Console");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());

	ImGui::End();

}
