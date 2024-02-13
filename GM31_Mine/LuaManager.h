#pragma once
#include <string>
#include <list>

#include <lua.hpp>
#include <luabind/luabind.hpp>

class Script;

class LuaManager
{
private:
	
	static LuaManager* m_Instance;	// シングルトン用インスタンス
	lua_State* m_L;
	bool m_isDebug;			// オブジェクトの関数をluabindのCall_functionでよび出した際にエラーをうまく拾えないため、オブジェクトの関数を呼ぶ場合はコールバック関数から直接エラーを拾う
	std::string m_errText;	// 拾ったエラー文

	std::list<Script*> m_scriptList;	// 現在のシーンのスクリプトのリスト
	
	LuaManager();	// コンストラクタ
	LuaManager(const LuaManager& manager);	// コピーコンストラクタ
	LuaManager& operator=(const LuaManager& manager);	// 代入演算子

	void BindClass();	// Luaスクリプトで使うクラスや関数を登録する関数



public:
	
	virtual ~LuaManager();	// デストラクタ
	
	static LuaManager* GetInstance();

	// Get系関数
	lua_State* GetLuaState() { return m_L; }
	bool GetIsDebug() { return m_isDebug; }
	std::string GetErrorText() { return m_errText; }

	// Set系関数
	void SetIsDebug(bool flag) { m_isDebug = flag; }
	void SetErrorText(std::string m) { m_errText = m; }

	// スクリプトリスト操作用関数
	void AddScriptList(Script* script);
	void DeleteScriptList(Script* script);
	void ClearScriptList();

	void CheckUpdateScript();		// スクリプトファイル（Luaファイル）が変更されているかをチェックする関数

	void CreateScriptFile(std::string fileName);

	void Init();	// 初期化処理
	void Uninit();	// 終了処理

};

int CallBack_Error(lua_State* L);	// Luaでエラーを検知した際に呼ばれるコールバック関数
