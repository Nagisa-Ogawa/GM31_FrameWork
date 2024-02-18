#pragma once
#include "MyImGui.h"


//-----------------------------------------------------
// デバッグ用ウィンドウGUIクラス
//------------------------------------------------------
class EngineDebugGui : public MyImGui
{
private:
	bool m_isShow = false;
public:
	void Init() override;	// 初期化関数
	void Update() override;	// 毎フレーム処理する関数
};
