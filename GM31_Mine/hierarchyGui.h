#pragma once
#include <string>
#include <map>
#include "MyImGui.h"


class HierarchyGui : public MyImGui
{
private:
	std::map<std::string, bool> m_gameObjecMap;
public:
	void Init() override;	// ‰Šú‰»ŠÖ”
	void Update()override;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”
};


