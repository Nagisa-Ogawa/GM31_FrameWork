#pragma once
#include "MyImGui.h"

class GameObject;

class InspectorGui : public MyImGui
{
private:
	GameObject* m_forcusObject{};
public:
	void Init() override;	// ‰Šú‰»ŠÖ”
	void Update()override;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”

	GameObject* GetForcusObject() { return m_forcusObject; }
	void SetForcusObject(GameObject* object) { m_forcusObject = object; }
};


