#pragma once
#include <string>
#include "MyImGui.h"


//-----------------------------------------------------
// �f�o�b�O�p�R���\�[���E�B���h�EGUI�N���X
//------------------------------------------------------
// ���b�Z�[�W��������\���o���܂���
//------------------------------------------------------
class ConsoleGui : public MyImGui
{
private:
	std::string m_message;
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	void SetMessage(std::string message) { m_message = message; }
};

