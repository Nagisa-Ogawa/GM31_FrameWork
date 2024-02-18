#include "myImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "boxCollisionFrame.h"
#include "engineDebugGui.h"
#include "fileDialog.h"

void EngineDebugGui::Init()
{
}

void EngineDebugGui::Update()
{
	ImGui::Begin("EngineDebug");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::End();
}
