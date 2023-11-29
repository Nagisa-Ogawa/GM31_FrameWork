#pragma once

#include <list>

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include <tchar.h>


class GameObject;
class MyImGui;

class MyImGuiManager
{
private:
	
	static MyImGuiManager* m_instance;	// シングルトン用インスタンス
	
	std::list<MyImGui*> m_myImGuiList;

	bool		m_isShowColl{};


	MyImGuiManager();	// コンストラクタ
	MyImGuiManager(const MyImGuiManager& manager);	// コピーコンストラクタ
	MyImGuiManager& operator=(const MyImGuiManager& manager);	// 代入演算子

public:
	
	virtual ~MyImGuiManager();	// デストラクタ
	void Init(HWND hwnd);		// 初期化処理
	void Uninit();				// 終了処理
	void Update();				// 更新処理
	void Draw();				// 描画処理

	/// <summary>
	/// ImGuiのWindowを追加
	/// </summary>
	/// <typeparam name="T">MyImGuiを継承したクラス</typeparam>
	/// <returns>MyImGuiを継承したクラス</returns>
	template <typename T>
	T* AddImGui() {
		MyImGui* myImGui = new T();
		m_myImGuiList.push_back(myImGui);
		myImGui->Init();

		return (T*)myImGui;
	}

	/// <summary>
	/// ImGuiを取得
	/// </summary>
	/// <typeparam name="T">MyImGuiを継承したクラス</typeparam>
	/// <returns>MyImGuiを継承したクラス</returns>
	template <typename T>
	T* GetImGui() {
		for (MyImGui* myImGui : m_myImGuiList)
		{
			if (typeid(*myImGui) == typeid(T))// 型を調べる(RTTI動的型情報)
			{
				return (T*)myImGui;
			}
		}
		return nullptr;

	}


	static MyImGuiManager* GetInstance();
};

