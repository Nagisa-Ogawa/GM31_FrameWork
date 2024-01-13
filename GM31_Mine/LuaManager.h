#pragma once
#include <string>

#include <lua.hpp>
#include <luabind/luabind.hpp>


class LuaManager
{
private:
	
	static LuaManager* m_Instance;	// �V���O���g���p�C���X�^���X
	lua_State* m_L;
	bool m_isDebug;			// �I�u�W�F�N�g�̊֐���luabind��Call_function�ł�яo�����ۂɃG���[�����܂��E���Ȃ����߁A�I�u�W�F�N�g�̊֐����Ăԏꍇ�̓R�[���o�b�N�֐����璼�ڃG���[���E��
	std::string m_errText;	// �E�����G���[��
	
	LuaManager();	// �R���X�g���N�^
	LuaManager(const LuaManager& manager);	// �R�s�[�R���X�g���N�^
	LuaManager& operator=(const LuaManager& manager);	// ������Z�q

	void BindClass();	// Lua�X�N���v�g�Ŏg���N���X��֐���o�^����֐�



public:
	
	virtual ~LuaManager();	// �f�X�g���N�^
	
	static LuaManager* GetInstance();

	lua_State* GetLuaState() { return m_L; }
	bool GetIsDebug() { return m_isDebug; }
	std::string GetErrorText() { return m_errText; }

	void SetIsDebug(bool flag) { m_isDebug = flag; }
	void SetErrorText(std::string m) { m_errText = m; }


	void Init();	// ����������
	void Uninit();	// �I������

};

int CallBack_Error(lua_State* L);	// Lua�ŃG���[�����m�����ۂɌĂ΂��R�[���o�b�N�֐�
