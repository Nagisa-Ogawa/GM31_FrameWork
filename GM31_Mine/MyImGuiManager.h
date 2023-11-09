#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include <tchar.h>

class GameObject;

class MyImGuiManager
{
private:
	// �V���O���g���p�C���X�^���X
	static MyImGuiManager* m_Instance;

	bool		m_IsShowColl{};
	

	GameObject* m_InfoObj{};

	// �R���X�g���N�^
	MyImGuiManager();
	// �R�s�[�R���X�g���N�^
	MyImGuiManager(const MyImGuiManager& manager);
	// ������Z�q
	MyImGuiManager& operator=(const MyImGuiManager& manager);

public:
	// �f�X�g���N�^
	virtual ~MyImGuiManager();
	// ����������
	void Init(HWND hwnd);
	void Uninit();
	void Update();
	void Draw();

	GameObject* GetMousePosObject();

	static MyImGuiManager* GetInstance();
};