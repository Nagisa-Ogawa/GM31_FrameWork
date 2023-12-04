#pragma once
#include "MyImGui.h"

class GameObject;

class InspectorGui : public MyImGui
{
private:
	GameObject* m_selectedObject{};
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	GameObject* GetForcusObject() { return m_selectedObject; }
	void SetForcusObject(GameObject* object) { m_selectedObject = object; }
};


