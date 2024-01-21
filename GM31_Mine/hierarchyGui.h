#pragma once
#include <string>
#include "MyImGui.h"


class HierarchyGui : public MyImGui
{
private:
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	void SetSelect(GameObject* object);
};


