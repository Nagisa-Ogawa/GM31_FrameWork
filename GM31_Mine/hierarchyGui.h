#pragma once
#include <string>
#include "MyImGui.h"


class HierarchyGui : public MyImGui
{
private:
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	void SetSelect(GameObject* object);
};


