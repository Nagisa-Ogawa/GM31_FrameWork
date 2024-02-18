#pragma once
#include <string>
#include "MyImGui.h"


//-----------------------------------------------------
// デバッグ用コンソールウィンドウGUIクラス
//------------------------------------------------------
// メッセージを一つしか表示出来ません
//------------------------------------------------------
class ConsoleGui : public MyImGui
{
private:
	std::string m_message;
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数

	void SetMessage(std::string message) { m_message = message; }
};

