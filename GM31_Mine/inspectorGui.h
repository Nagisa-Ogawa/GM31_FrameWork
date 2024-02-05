#pragma once
#include "MyImGui.h"

class GameObject;

class InspectorGui : public MyImGui
{
private:
	GameObject* m_selectedObject{};
	bool m_isAddComp = false;
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	GameObject* GetSelectedObject() { return m_selectedObject; }
	void SetSelectedObject(GameObject* object) { m_selectedObject = object; }
};


