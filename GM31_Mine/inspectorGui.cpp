#include "MyImGuiManager.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "MyImGuiManager.h"
#include "dispInspector.h"
#include "inspectorGui.h"
#include "SceneGui.h"

void InspectorGui::Init()
{
}

void InspectorGui::Update()
{
	
	ImGui::Begin("Inspecter");
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	if (m_selectedObject) {
		// 選択されているオブジェクトにコンポーネントされているクラスを取得
		auto componentList = m_selectedObject->GetAllComponent();
		// コンポーネントのクラスに応じてインスペクターを表示する関数を呼ぶ
		for (auto component : componentList) {
			component->DispInspector();
		}
	}
	//if (m_selectedObject) {

	//}
	ImGui::End();

}
