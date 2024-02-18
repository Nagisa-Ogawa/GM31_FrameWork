#pragma once
#include "MyImGui.h"

class GameObject;


//-----------------------------------------------------
// メニューバー用GUIクラス
//------------------------------------------------------
class MainMenuBarGui : public MyImGui
{
private:
	ID3D11ShaderResourceView* m_executeBtnTexture = nullptr;
	ID3D11ShaderResourceView* m_stopBtnTexture = nullptr;
public:
	void Init() override;	// 初期化関数
	void Uninit() override;
	void Update()override;	// 毎フレーム処理する関数

};


