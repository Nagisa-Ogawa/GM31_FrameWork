#pragma once
#include "MyImGui.h"


//-----------------------------------------------------
// ゲームウィンドウ用GUIクラス
//------------------------------------------------------
class GameGui : public MyImGui
{
private:
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

};


