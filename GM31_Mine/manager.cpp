#include <fstream>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "LuaManager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
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
	m_mode = ENGINE_MODE::EDIT;

	Renderer::Init();
	MyImGuiManager::GetInstance()->Init(GetWindow());
	LuaManager::GetInstance()->Init();
	
	// Scene�t�@�C���̒����m�F
	if (CheckSceneFile()) {
		// Scene�̃f�[�^���������Ȃ�ǂݍ��ݏ���
		LoadScene();
	}
	else {
		// Scene�̃f�[�^���Ȃ��Ȃ�V�[�����쐬
		auto scene = std::make_shared<Scene>();
		m_scene = scene.get();
		m_scene->SetName("Test");
		m_sceneList.push_back(scene);
		m_editor = new Editor();
		m_scene->Init();
		m_editor->Init();
	}

}

void Manager::Uninit()
{
	m_scene->Uninit();
	m_editor->Uninit();
	delete m_editor;
	LuaManager::GetInstance()->Uninit();
	MyImGuiManager::GetInstance()->Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{

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

}

void Manager::Draw()
{
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

}

bool Manager::CheckSceneFile()
{
	return false;
}

/// <summary>
/// �V�[�����t�@�C���ɕۑ�����֐�
/// </summary>
/// <param name="fileName">�ۑ�����t�@�C����</param>
void Manager::SaveScene()
{
	// ���݂���V�[�������ׂĕۑ�����
	for (auto scene : m_sceneList) {
		// Scenes�t�H���_�ɃV�[���t�@�C�����쐬
		std::string filePath = "Scenes\\"+scene->GetName() + ".json";
		std::ofstream file(filePath);
		// �V�[�������V���A���C�Y
		cereal::JSONOutputArchive archive(file);
		try {
			archive(scene);
		}
		catch (std::exception& e) {
			MyImGuiManager::GetInstance()->DebugLog(e.what());
		}
	}
}

/// <summary>
/// �V�[�������[�h����֐�
/// </summary>
void Manager::LoadScene()
{
	// �t�@�C������V�[�������[�h
}
