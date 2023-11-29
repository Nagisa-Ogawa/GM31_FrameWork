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
	T* AddImGui() {
		MyImGui* myImGui = new T();
		m_myImGuiList.push_back(myImGui);
		myImGui->Init();

		return (T*)myImGui;
	}

	/// <summary>
	/// ImGui���擾
	/// </summary>
	/// <typeparam name="T">MyImGui���p�������N���X</typeparam>
	/// <returns>MyImGui���p�������N���X</returns>
	template <typename T>
	T* GetImGui() {
		for (MyImGui* myImGui : m_myImGuiList)
		{
			if (typeid(*myImGui) == typeid(T))// �^�𒲂ׂ�(RTTI���I�^���)
			{
				return (T*)myImGui;
			}
		}
		return nullptr;

	}


	static MyImGuiManager* GetInstance();
};

