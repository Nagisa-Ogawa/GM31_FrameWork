#pragma once
#include <string>
#include "MyImGui.h"


class ConsoleGui : public MyImGui
{
private:
	std::string m_message;
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�

	void SetMessage(std::string message) { m_message = message; }
};

