#pragma once
#include "MyImGui.h"


//-----------------------------------------------------
// �f�o�b�O�p�E�B���h�EGUI�N���X
//------------------------------------------------------
class EngineDebugGui : public MyImGui
{
private:
	bool m_isShow = false;
public:
	void Init() override;	// �������֐�
	void Update() override;	// ���t���[����������֐�
};
