#include "ImGui/imgui.h"
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
	ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
	ImGui::Text(" ObjectCount : %d", Manager::GetInstance()->GetScene()->GetGameObjectCount());
	ImGui::Text(" ActiveObjectCount : %d", Manager::GetInstance()->GetScene()->GetActiveGameObjectCount());
	bool isShow = false;
	if (ImGui::Checkbox(" Show Collision", &isShow)) {
		auto colls = Manager::GetInstance()->GetScene()->GetGameObjects<BoxCollisionFrame>();
		for (auto coll : colls) {
			coll->SetActive(isShow);
		}
	}
	ImVec2 mousePos = ImGui::GetMousePos();
	ImGui::Text("mousePos : x %f   y %f", mousePos.x, mousePos.y);
	ImGui::End();
}
