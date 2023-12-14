#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "game.h"
#include "editor.h"
#include "title.h"

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
	m_mode = ENGINE_MODE::EDIT;
	m_gameMode = GAME_MODE::TITLE;
	m_nextMode = GAME_MODE::NONE;
	Renderer::Init();
	m_editor = new Editor();
	m_editor->Init();
	m_scene = new Game();
	m_scene->Init();
	m_title = new Title();
	m_title->Init();
	Input::Init();
}

void Manager::Uninit()
{
	m_scene->Uninit();
	delete m_scene;
	m_editor->Uninit();
	delete m_editor;
	m_title->Uninit();
	delete m_title;
	Renderer::Uninit();
}

void Manager::Update()
{
	if (m_nextMode!=GAME_MODE::NONE)
	{
		m_gameMode = m_nextMode;
		m_nextMode = GAME_MODE::NONE;
	}
	switch (m_gameMode)
	{
	case TITLE:
		Input::Update();
		m_title->Update();
		break;
	case INGAME:

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
			Input::Update();
			m_scene->Update();

			break;
		case PAUSE:
			// �ꎞ��~���̓G�f�B�^�̂ݍX�V���u���Ăяo��
			m_editor->Update();
			break;
		default:
			break;
		}

		// GUI�̍X�V����
		MyImGuiManager::GetInstance()->Update();
		break;
	default:
		break;
	}

}

void Manager::Draw()
{
	switch (m_gameMode)
	{
	case TITLE:
		Renderer::Begin();
		m_title->Draw();
		Renderer::End();
		break;
	case INGAME:
		// �e�N�X�`���ɃG�f�B�^��ʂ������_�����O
		Renderer::EditorViewBegin();

		m_editor->Draw();
		m_scene->Draw();

		// �e�N�X�`���Ɏ��s��ʂ������_�����O
		Renderer::GameViewBegin();

		m_scene->Draw();

		// ��ʑS�̂������_�����O
		Renderer::Begin();

		MyImGuiManager::GetInstance()->Draw();

		Renderer::End();
		break;
	default:
		break;
	}

}
