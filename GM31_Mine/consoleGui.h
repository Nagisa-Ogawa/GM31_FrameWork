#pragma once
#include <string>
#include "MyImGui.h"


class ConsoleGui : public MyImGui
{
private:
	std::string m_message;
public:
	void Init() override;	// ‰Šú‰»ŠÖ”
	void Update()override;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”

	void SetMessage(std::string message) { m_message = message; }
};

