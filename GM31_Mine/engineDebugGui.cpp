#include "MyImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "boxCollisionFrame.h"
#include "engineDebugGui.h"

void EngineDebugGui::Init()
{
}

void EngineDebugGui::Update()
{
	ImGui::Begin("EngineDebug");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	bool isShow = false;
	if (ImGui::Checkbox(" Show Collision", &isShow)) {
		auto colls = Manager::GetInstance()->GetScene()->GetGameObjects<BoxCollisionFrame>();
		for (auto coll : colls) {
			coll->SetActive(isShow);
		}
	}

	ImGui::Text("FocusWindowName : %s", MyImGuiManager::GetInstance()->GetFocusWindow()->Name);
	ImGui::End();
}
