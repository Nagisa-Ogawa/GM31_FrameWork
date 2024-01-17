#pragma once
#include "MyImGui.h"


class EngineDebugGui : public MyImGui
{
private:
	bool m_isShow = false;
public:
	void Init() override;	// ‰Šú‰»ŠÖ”
	void Update() override;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”
};
