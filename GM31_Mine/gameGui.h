#pragma once
#include "MyImGui.h"


class GameGui : public MyImGui
{
private:
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

};


