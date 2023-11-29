#pragma once
#include "MyImGui.h"

class GameObject;

class InspectorGui : public MyImGui
{
private:
	GameObject* m_forcusObject{};
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	GameObject* GetForcusObject() { return m_forcusObject; }
	void SetForcusObject(GameObject* object) { m_forcusObject = object; }
};


