
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
	// ����������
	m_fileName = fileName;
	// ���X�g�ɒǉ�
	LuaManager::GetInstance()->AddScriptList(this);
	// �t�@�C���̍X�V���Ԃ��m�F
	m_updateTime = GetUpdateTime();
	// �ŏ���Lua�t�@�C�����R���p�C��
	CompileLua();

	// �Q�[�����ɏ������������Ă΂ꂽ�ꍇ��Start�֐����Ăяo��
	if (Manager::GetInstance()->GetMode() == ENGINE_MODE::RUN) {
		Start();
	}
}

void Script::Load()
{
	// ���X�g�ɒǉ�
	LuaManager::GetInstance()->AddScriptList(this);
	// �t�@�C���̍X�V���Ԃ��m�F
	m_updateTime = GetUpdateTime();
	// Lua�t�@�C�����R���p�C��
	CompileLua();
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
	LuaManager::GetInstance()->DeleteScriptList(this);
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
/// �X�N���v�g�t�@�C����ύX����֐�
/// </summary>
void Script::ChangeScriptFile(std::string filePath)
{
	// �t�@�C�����������Ȃ�ύX���Ȃ�
	if (m_fileName == filePath) {
		return;
	}
	m_fileName = filePath;
	// �t�@�C���̍X�V���Ԃ��m�F
	m_updateTime = GetUpdateTime();
	// Lua�t�@�C�����R���p�C��
	CompileLua();
}

/// <summary>
/// Lua�t�@�C�����R���p�C������֐�
/// </summary>
void Script::CompileLua()
{
	lua_State* L = LuaManager::GetInstance()->GetLuaState();

	// ���삷��O�̃X�^�b�N���L�^
	int top = lua_gettop(L);
	LuaManager::GetInstance()->SetIsDebug(true);

	// �t�@�C�������[�h
	if (luaL_loadfile(L, m_fileName.c_str()) != 0) {
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

/// <summary>
/// Lua�t�@�C���̍X�V���Ԃ��擾����֐�
/// </summary>
time_t Script::GetUpdateTime()
{
	struct _stat buf;
	time_t updateTime;
	std::string filepath = m_fileName;
	// �t�@�C���̏����擾����
	int err = _stat(filepath.c_str(), &buf);
	if (err == 0) {
		// �X�V���Ԃ��擾
		updateTime = buf.st_mtime;
		return updateTime;
	}
	else {
		return 0;
	}
}


/// <summary>
/// Lua�t�@�C�����X�V����Ă��邩���`�F�b�N����֐�
/// </summary>
/// <returns>�X�V����Ă��邩�ǂ���</returns>
bool Script::CheckUpdate()
{
	// ���݂̃t�@�C���X�V���Ԃ��擾
	auto nowUpdateTime = GetUpdateTime();
	// �X�V���Ԃ��ύX����Ă����Ȃ�Lua�t�@�C�����X�V����Ă���
	if (nowUpdateTime != 0 && m_updateTime != nowUpdateTime) {
		return true;
	}
	return false;
}
