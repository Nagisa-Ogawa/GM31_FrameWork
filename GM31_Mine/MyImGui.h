#pragma once

class MyImGui 
{
private:
public:
	virtual void Init() = 0;	// 初期化関数
	virtual void Update() = 0;	// 毎フレーム処理する関数
};
