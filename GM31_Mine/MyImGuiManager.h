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

	bool		m_IsShowColl{};
	

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

	GameObject* GetMousePosObject();

	static MyImGuiManager* GetInstance();
};