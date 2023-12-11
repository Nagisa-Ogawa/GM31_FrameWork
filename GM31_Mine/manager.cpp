#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "game.h"
#include "editor.h"

Manager* Manager::m_instance = NULL;

Manager::Manager()
{
	// �Ȃɂ����Ȃ�
}

Manager::Manager(const Manager& manager)
{
	// �C���X�^���X���R�s�[
	m_instance = manager.m_instance;
}

Manager& Manager::operator=(const Manager& manager)
{
	// �C���X�^���X���R�s�[
	m_instance = manager.m_instance;
	return *this;
}

Manager::~Manager()
{
	// �C���X�^���X�����
	delete m_instance;
}

Manager* Manager::GetInstance()
{
	// ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
	// ����ȍ~�͐��������C���X�^���X��n��
	if (m_instance == NULL)
	{
		m_instance = new Manager();
		m_instance->Init();
	}
	return m_instance;
}

void Manager::Init()
{
	m_mode = EDITOR_MODE::EDIT;

	Renderer::Init();
	m_editor = new Editor();
	m_editor->Init();
	m_scene = new Game();
	m_scene->Init();
	
	Input::Init();
}

void Manager::Uninit()
{
	m_scene->Uninit();
	delete m_scene;
	m_editor->Uninit();
	delete m_editor;
	Renderer::Uninit();
}

void Manager::Update()
{

	Input::Update();

	//  �Q�[���G���W���̏�Ԃɍ��킹�ĕ���
	switch (m_mode)
	{
	case EDIT:
		// �ҏW���[�h�̎��̓G�f�B�^�̂ݍX�V�������Ăяo��
		m_editor->Update();
		break;
	case RUN:
		// ���s���̎��̓G�f�B�^�ƃQ�[���̍X�V�������ǂ�����Ăяo��
		m_editor->Update();

		if (m_nextScene)
		{
			if (m_scene)
			{
				m_scene->Uninit();
				delete m_scene;
			}
			m_scene = m_nextScene;
			m_scene->Init();
			m_nextScene = nullptr;
		}
		m_scene->Update();

		break;
	default:
		break;
	}
	MyImGuiManager::GetInstance()->Update();

}

void Manager::Draw()
{
	// �e�N�X�`���ɃG�f�B�^��ʂ������_�����O
	Renderer::EditorViewBegin();

	m_editor->Draw();
	m_scene->Draw();

	// �e�N�X�`���Ɏ��s��ʂ������_�����O


	// ��ʑS�̂������_�����O
	Renderer::Begin();

	MyImGuiManager::GetInstance()->Draw();

	Renderer::End();

}
