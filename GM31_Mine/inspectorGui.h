#pragma once
#include "MyImGui.h"

class GameObject;

class InspectorGui : public MyImGui
{
private:
	GameObject* m_selectedObject{};
public:
	void Init() override;	// ‰Šú‰»ŠÖ”
	void Update()override;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”

	GameObject* GetForcusObject() { return m_selectedObject; }
	void SetForcusObject(GameObject* object) { m_selectedObject = object; }
};


