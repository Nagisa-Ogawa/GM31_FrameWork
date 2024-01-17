
#include <iostream>
#include <fstream>
#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "LuaManager.h"
#include "MyImGuiManager.h"
#include "script.h"
#include "dispInspector.h"


void Script::Init(std::string fileName)
{
	// 初期化処理
	m_fileName = fileName;
	// とりあえず最初にLuaファイルをコンパイル
	CompileLua();

	// ゲーム中に初期化処理が呼ばれた場合はStart関数を呼び出す
	if (Manager::GetInstance()->GetMode() == ENGINE_MODE::RUN) {
		Start();
	}
}

/// <summary>
/// シーンが実行された時に一度だけ呼び出される関数
/// 
/// </summary>
void Script::Start()
{
	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// 操作する前のスタックを記録
	int top = lua_gettop(L);
	
	// Start関数を呼び出す
	try {
		LuaManager::GetInstance()->SetIsDebug(false);
		luabind::call_function<void>(m_object["Start"]);
	}
	catch (const luabind::error& e) {
		lua_pop(L, 1);
		std::string m = m_fileName + " call Start() function failed!\n" + LuaManager::GetInstance()->GetErrorText();
		MyImGuiManager::GetInstance()->DebugLog(m);
		return;
	}

	// スタックをリセットする
	lua_settop(L, top);

}


void Script::Uninit()
{
}

void Script::Update()
{

	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// 操作する前のスタックを記録
	int top = lua_gettop(L);

	// Update関数を呼び出す
	try {
		LuaManager::GetInstance()->SetIsDebug(false);
		luabind::call_function<void>(m_object["Update"]);
	}
	catch (const luabind::error& e) {
		lua_pop(L, 1);
		std::string m = m_fileName + " call Update() function failed!\n" + LuaManager::GetInstance()->GetErrorText();
		MyImGuiManager::GetInstance()->DebugLog(m);
	}

	// スタックをリセットする
	lua_settop(L, top);

}

void Script::Draw()
{
	// 何もしない
}

void Script::DispInspector()
{
	DispComponent(this);
}

/// <summary>
/// Luaファイルをコンパイルする関数
/// </summary>
void Script::CompileLua()
{
	std::string fileName = "Scripts\\" + m_fileName;
	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// 操作する前のスタックを記録
	int top = lua_gettop(L);
	LuaManager::GetInstance()->SetIsDebug(true);

	// ファイルをロード
	if (luaL_loadfile(L, fileName.c_str()) != 0) {
		std::string m = m_fileName + " luaL_loadfile failed!\n" + lua_tostring(L, -1);
		MyImGuiManager::GetInstance()->DebugLog(m);
		lua_pop(L, 1);
		return;
	}
	// Luaファイルをコンパイル
	if (lua_pcall(L, 0, 1, 0) != 0) {
		std::string m = m_fileName + " lua_pcall failed!\n" + lua_tostring(L, -1);
		MyImGuiManager::GetInstance()->DebugLog(m);
		lua_pop(L, 1);
		return;
	}
	// Objectを取得
	luabind::object data(luabind::from_stack(L, -1));
	m_object = data;
	lua_pop(L, 1);
	// Luaスクリプトのコンポーネント元のゲームオブジェクトを渡す
	try {
		luabind::call_function<void>(L,"GetGameObject", m_gameObject);
	}
	catch (const luabind::error& e) {
		luabind::object errorMessage(luabind::from_stack(e.state(), -1));
		lua_pop(L, 1);
		std::string m = m_fileName + " GetGameObject Function failed!\n" + luabind::object_cast<std::string>(errorMessage);
		MyImGuiManager::GetInstance()->DebugLog(m);
		return;
	}

	// スタックをリセットする
	lua_settop(L, top);

}