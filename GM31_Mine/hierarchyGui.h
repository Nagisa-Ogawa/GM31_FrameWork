#pragma once
#include <string>
#include "MyImGui.h"


class HierarchyGui : public MyImGui
{
private:
public:
	void Init() override;	// ‰Šú‰»ŠÖ”
	void Update()override;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”

	void SetSelect(GameObject* object);
};


