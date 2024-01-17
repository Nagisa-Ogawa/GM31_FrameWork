
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
	// ����������
	m_fileName = fileName;
	// �Ƃ肠�����ŏ���Lua�t�@�C�����R���p�C��
	CompileLua();

	// �Q�[�����ɏ������������Ă΂ꂽ�ꍇ��Start�֐����Ăяo��
	if (Manager::GetInstance()->GetMode() == ENGINE_MODE::RUN) {
		Start();
	}
}

/// <summary>
/// �V�[�������s���ꂽ���Ɉ�x�����Ăяo�����֐�
/// 
/// </summary>
void Script::Start()
{
	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// ���삷��O�̃X�^�b�N���L�^
	int top = lua_gettop(L);
	
	// Start�֐����Ăяo��
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

	// �X�^�b�N�����Z�b�g����
	lua_settop(L, top);

}


void Script::Uninit()
{
}

void Script::Update()
{

	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// ���삷��O�̃X�^�b�N���L�^
	int top = lua_gettop(L);

	// Update�֐����Ăяo��
	try {
		LuaManager::GetInstance()->SetIsDebug(false);
		luabind::call_function<void>(m_object["Update"]);
	}
	catch (const luabind::error& e) {
		lua_pop(L, 1);
		std::string m = m_fileName + " call Update() function failed!\n" + LuaManager::GetInstance()->GetErrorText();
		MyImGuiManager::GetInstance()->DebugLog(m);
	}

	// �X�^�b�N�����Z�b�g����
	lua_settop(L, top);

}

void Script::Draw()
{
	// �������Ȃ�
}

void Script::DispInspector()
{
	DispComponent(this);
}

/// <summary>
/// Lua�t�@�C�����R���p�C������֐�
/// </summary>
void Script::CompileLua()
{
	std::string fileName = "Scripts\\" + m_fileName;
	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// ���삷��O�̃X�^�b�N���L�^
	int top = lua_gettop(L);
	LuaManager::GetInstance()->SetIsDebug(true);

	// �t�@�C�������[�h
	if (luaL_loadfile(L, fileName.c_str()) != 0) {
		std::string m = m_fileName + " luaL_loadfile failed!\n" + lua_tostring(L, -1);
		MyImGuiManager::GetInstance()->DebugLog(m);
		lua_pop(L, 1);
		return;
	}
	// Lua�t�@�C�����R���p�C��
	if (lua_pcall(L, 0, 1, 0) != 0) {
		std::string m = m_fileName + " lua_pcall failed!\n" + lua_tostring(L, -1);
		MyImGuiManager::GetInstance()->DebugLog(m);
		lua_pop(L, 1);
		return;
	}
	// Object���擾
	luabind::object data(luabind::from_stack(L, -1));
	m_object = data;
	lua_pop(L, 1);
	// Lua�X�N���v�g�̃R���|�[�l���g���̃Q�[���I�u�W�F�N�g��n��
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

	// �X�^�b�N�����Z�b�g����
	lua_settop(L, top);

}