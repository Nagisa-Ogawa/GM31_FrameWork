#pragma once
#include "MyImGui.h"


class EngineDebugGui : public MyImGui
{
private:
	bool m_isShow = false;
public:
	void Init() override;	// �������֐�
	void Update() override;	// ���t���[����������֐�
};
