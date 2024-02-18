
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include "main.h"
#include "manager.h"
#include "gameObject.h"
#include "luaManager.h"
#include "myImGuiManager.h"
#include "script.h"
#include "dispInspector.h"


void Script::Init(std::string fileName)
{
	// 初期化処理
	m_fileName = fileName;
	// リストに追加
	LuaManager::GetInstance()->AddScriptList(this);
	// ファイルの更新時間を確認
	m_updateTime = GetUpdateTime();
	// 最初にLuaファイルをコンパイル
	CompileLua();

	// ゲーム中に初期化処理が呼ばれた場合はStart関数を呼び出す
	if (Manager::GetInstance()->GetMode() == ENGINE_MODE::RUN) {
		Start();
	}
}

void Script::Load()
{
	// リストに追加
	LuaManager::GetInstance()->AddScriptList(this);
	// ファイルの更新時間を確認
	m_updateTime = GetUpdateTime();
	// Luaファイルをコンパイル
	CompileLua();
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
	LuaManager::GetInstance()->DeleteScriptList(this);
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
/// スクリプトファイルを変更する関数
/// </summary>
void Script::ChangeScriptFile(std::string filePath)
{
	// ファイル名が同じなら変更しない
	if (m_fileName == filePath) {
		return;
	}
	m_fileName = filePath;
	// ファイルの更新時間を確認
	m_updateTime = GetUpdateTime();
	// Luaファイルをコンパイル
	CompileLua();
}

/// <summary>
/// Luaファイルをコンパイルする関数
/// </summary>
void Script::CompileLua()
{
	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// 操作する前のスタックを記録
	int top = lua_gettop(L);
	LuaManager::GetInstance()->SetIsDebug(true);

	// ファイルをロード
	if (luaL_loadfile(L, m_fileName.c_str()) != 0) {
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

/// <summary>
/// Luaファイルの更新時間を取得する関数
/// </summary>
time_t Script::GetUpdateTime()
{
	struct _stat buf;
	time_t updateTime;
	std::string filepath = m_fileName;
	// ファイルの情報を取得する
	int err = _stat(filepath.c_str(), &buf);
	if (err == 0) {
		// 更新時間を取得
		updateTime = buf.st_mtime;
		return updateTime;
	}
	else {
		return 0;
	}
}


/// <summary>
/// Luaファイルが更新されているかをチェックする関数
/// </summary>
/// <returns>更新されているかどうか</returns>
bool Script::CheckUpdate()
{
	// 現在のファイル更新時間を取得
	auto nowUpdateTime = GetUpdateTime();
	// 更新時間が変更されていたならLuaファイルが更新されている
	if (nowUpdateTime != 0 && m_updateTime != nowUpdateTime) {
		return true;
	}
	return false;
}
