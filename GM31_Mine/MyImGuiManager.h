#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include <tchar.h>

class GameObject;

class MyImGuiManager
{
private:
	// シングルトン用インスタンス
	static MyImGuiManager* m_Instance;

	bool		m_isShowColl{};

	GameObject* m_InfoObj{};

	// コンストラクタ
	MyImGuiManager();
	// コピーコンストラクタ
	MyImGuiManager(const MyImGuiManager& manager);
	// 代入演算子
	MyImGuiManager& operator=(const MyImGuiManager& manager);

public:
	// デストラクタ
	virtual ~MyImGuiManager();
	// 初期化処理
	void Init(HWND hwnd);
	void Uninit();
	void Update();
	void Draw();


	POINT		ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize);	// 通常の画面で取得した座標をゲーム画面をレンダリングしている画面での座標に変換
	GameObject* GetMousePosObject(POINT mousePos);		// マウス座標にオブジェクトがあるかを調べそのオブジェクトを返す関数

	static MyImGuiManager* GetInstance();
};