#pragma once
#include "MyImGui.h"

class GameObject;

class InspectorGui : public MyImGui
{
private:
	GameObject* m_selectedObject{};
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	GameObject* GetForcusObject() { return m_selectedObject; }
	void SetForcusObject(GameObject* object) { m_selectedObject = object; }
};


