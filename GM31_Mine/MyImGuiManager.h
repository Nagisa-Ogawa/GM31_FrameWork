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
	
	static MyImGuiManager* m_instance;	// �V���O���g���p�C���X�^���X
	
	std::list<MyImGui*> m_myImGuiList;

	bool		m_isShowColl{};

	GameObject* m_infoObj{};

	MyImGuiManager();	// �R���X�g���N�^
	MyImGuiManager(const MyImGuiManager& manager);	// �R�s�[�R���X�g���N�^
	MyImGuiManager& operator=(const MyImGuiManager& manager);	// ������Z�q

public:
	
	virtual ~MyImGuiManager();	// �f�X�g���N�^
	void Init(HWND hwnd);		// ����������
	void Uninit();				// �I������
	void Update();				// �X�V����
	void Draw();				// �`�揈��

	/// <summary>
	/// ImGui��Window��ǉ�
	/// </summary>
	/// <typeparam name="T">MyImGui���p�������N���X</typeparam>
	/// <returns>MyImGui���p�������N���X</returns>
	template <typename T>
	T* AddImGui();

	/// <summary>
	/// ImGui���擾
	/// </summary>
	/// <typeparam name="T">MyImGui���p�������N���X</typeparam>
	/// <returns>MyImGui���p�������N���X</returns>
	template <typename T>
	T* GetImGui();

	POINT		ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize);	// �ʏ�̉�ʂŎ擾�������W���Q�[����ʂ������_�����O���Ă����ʂł̍��W�ɕϊ�
	GameObject* GetMousePosObject(POINT mousePos);		// �}�E�X���W�ɃI�u�W�F�N�g�����邩�𒲂ׂ��̃I�u�W�F�N�g��Ԃ��֐�

	static MyImGuiManager* GetInstance();
};

