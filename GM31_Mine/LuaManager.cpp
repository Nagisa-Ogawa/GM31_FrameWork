
#include "LuaManager.h"
#include <luabind/operator.hpp>

#include "MyImGuiManager.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "gameObject.h"
#include "transform.h"
#include "script.h"


LuaManager* LuaManager::m_Instance = NULL;

LuaManager::LuaManager()
{
	// なにもしない
}

LuaManager::LuaManager(const LuaManager& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
}

LuaManager& LuaManager::operator=(const LuaManager& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
	return *m_Instance;
}

/// <summary>
/// Luaスクリプトで使うクラスやインスタンスを登録する関数
/// </summary>
void LuaManager::BindClass()
{
	// クラスを登録
	luabind::module(m_L)
		[
			luabind::class_<MyImGuiManager>("MyImGuiManager")
				.def("DebugLog", &MyImGuiManager::DebugLog),
			luabind::class_<D3DVECTOR>("Vector")
				.def_readwrite("x", &D3DVECTOR::x)
				.def_readwrite("y", &D3DVECTOR::y)
				.def_readwrite("z", &D3DVECTOR::z),
			luabind::class_<D3DXVECTOR3, D3DVECTOR>("Vector3")
				.def(luabind::constructor<>())
				.def(luabind::constructor<FLOAT, FLOAT, FLOAT>())
				.def(luabind::const_self + D3DXVECTOR3())
				.def(luabind::const_self - D3DXVECTOR3())
				.def(luabind::const_self * float()),
			luabind::class_<Transform>("Transform")
				.def("GetRight", &Transform::GetRight)
				.def("GetUp", &Transform::GetUp)
				.def("GetForward", &Transform::GetForward)
				.def_readwrite("worldPosition", &Transform::m_worldPosition)
				.def_readwrite("worldRotation", &Transform::m_worldRotation)
				.def_readwrite("worldScale", &Transform::m_worldScale)
				.def_readwrite("localPosition",&Transform::m_localPosition)
				.def_readwrite("localRotation", &Transform::m_localRotation)
				.def_readwrite("localScale", &Transform::m_localScale),
			luabind::class_<GameObject>("GameObject")
				.def("GetActive", &GameObject::GetActive)
				.def("GetName", &GameObject::GetName)
				.def("SetDestroy", &GameObject::SetDestroy)
				.def("SetActive", &GameObject::SetActive)
				.def("SetName", &GameObject::SetName)
				.def("GetTransform", &GameObject::GetTransform),
			luabind::class_<Input>("Input")
				.scope
				[
					luabind::def("GetKeyPress", &Input::GetKeyPress),
					luabind::def("GetKeyTrigger", &Input::GetKeyTrigger)
				]
		];

	// グローバル変数にdefine値を登録
	luabind::object g = luabind::globals(m_L);
	g["KEY_A"] = KEY_A;
	g["KEY_D"] = KEY_D;
	g["KEY_E"] = KEY_E;
	g["KEY_Q"] = KEY_Q;
	g["KEY_W"] = KEY_W;
	g["KEY_S"] = KEY_S;


	
	// グローバル変数に各種インスタンスを登録
	luabind::globals(m_L)["ImGuiManager"] = MyImGuiManager::GetInstance();
}


void LuaManager::Init()
{
	// 初期化
	m_L = luaL_newstate();
	luaL_openlibs(m_L);
	luabind::open(m_L);

	// クラスやグローバル関数を登録
	BindClass();

	// エラー時に呼ばれるコールバック関数を登録
	luabind::set_pcall_callback(CallBack_Error);
}

void LuaManager::Uninit()
{
	m_scriptList.clear();
	// 終了
	lua_close(m_L);
}

void LuaManager::AddScriptList(Script* script)
{
	if (script)
		m_scriptList.push_back(script);
}

void LuaManager::DeleteScriptList(Script* script)
{
	if (script)
		m_scriptList.remove(script);
}

void LuaManager::ClearScriptList()
{
	m_scriptList.clear();
}

/// <summary>
/// 現在のシーンで使用されているLuaファイル
/// </summary>
void LuaManager::CheckUpdateScript()
{
	// 現在のシーンにあるファイルをチェック
	for (auto script : m_scriptList) {
		// ファイルが変更されているかをチェック
		if (script->CheckUpdate()) {
			// 変更されていたならコンパイルをし直す
			MyImGuiManager::GetInstance()->DebugLog("Lua update !!");
			script->CompileLua();
		}
	}
}


LuaManager::~LuaManager()
{
	// インスタンスを解放
	delete m_Instance;
}

LuaManager* LuaManager::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
	if (m_Instance == NULL)
	{
		m_Instance = new LuaManager();
	}
	return m_Instance;
}


int CallBack_Error(lua_State* L)
{
	lua_Debug d;
	lua_getstack(L, 1, &d);
	lua_getinfo(L, "Sln", &d);
	std::string err = lua_tostring(L, -1);
	lua_pop(L, 1);
	std::stringstream msg;
	msg << d.short_src << ":" << d.currentline;

	if (d.name != 0)
	{
		msg << "(" << d.namewhat << " " << d.name << ")";
	}
	msg << " "<< err;
	if (LuaManager::GetInstance()->GetIsDebug()) {
		lua_pushstring(L, msg.str().c_str());
	}
	else {
		LuaManager::GetInstance()->SetErrorText(err);
	}
	return 1;
}
