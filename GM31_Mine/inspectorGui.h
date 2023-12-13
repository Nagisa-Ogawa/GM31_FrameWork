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

	GameObject* GetSelectedObject() { return m_selectedObject; }
	void SetSelectedObject(GameObject* object) { m_selectedObject = object; }
};


