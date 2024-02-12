#include <fstream>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "LuaManager.h"
#include "CollisionManager.h"
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
		auto scene = std::make_unique<Scene>();
		m_scene = scene.get();
		m_scene->SetName("Test");
		m_scene->Init();
		m_editor = m_scene->GetEditor();
		m_sceneList.push_back(std::move(scene));
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
		// Lua�t�@�C�����X�V����Ă��邩�`�F�b�N
		LuaManager::GetInstance()->CheckUpdateScript();
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

	// �I�u�W�F�N�g���j������Ă��邩���`�F�b�N
	m_scene->CheckDestroyedObject();

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


/// <summary>
/// �V�[�����t�@�C���ɕۑ�����֐�
/// </summary>
/// <param name="fileName">�ۑ�����t�@�C����</param>
void Manager::SaveScene()
{
	// ���݂���V�[�������ׂĕۑ�����
	for (const auto& scene : m_sceneList) {
		// Scenes�t�H���_�ɃV�[���t�@�C�����쐬
		std::string filePath = "Assets\\Scenes\\"+scene->GetName() + ".json";
		std::ofstream file(filePath);
		// �V�[�������V���A���C�Y
		try {
			cereal::JSONOutputArchive archive(file);
			archive(scene);
		}
		catch (std::exception& e) {
			MyImGuiManager::GetInstance()->DebugLog(e.what());
		}
	}
	// �I�������琬���������Ƃ�ʒm
	MyImGuiManager::GetInstance()->DebugLog("Save Successful !!");
}

/// <summary>
/// �V�[�������[�h����֐�
/// </summary>
void Manager::LoadScene()
{
	// �t�@�C������V�[�������[�h
	std::list<std::string> nameList = GetAllFiles("Assets\\Scenes");
	for (auto fileName : nameList) {
		std::unique_ptr<Scene> scene;
		try {
			std::ifstream file("Assets\\Scenes\\" + fileName);
			cereal::JSONInputArchive archive(file);
			archive(scene);
		}
		catch (std::exception& e) {
			MyImGuiManager::GetInstance()->DebugLog(e.what());
		}
		// �t�@�C������ǂݍ��񂾌�ɓǂݍ��݌�֐����Ăяo��
		m_scene = scene.get();
		m_editor = scene->GetEditor();
		scene->Load();
		m_sceneList.push_back(std::move(scene));
	}
	// �I�������琬���������Ƃ�ʒm
	MyImGuiManager::GetInstance()->DebugLog("Load Successful !!");
}

// �V�[�����Đ�����֐�
void Manager::PlayScene()
{
	// �Q�[���E�B���h�E�Ƀt�H�[�J�X��ݒ�
	ImGui::SetWindowFocus("Game");
	// ���݂̃V�[�����t�@�C���ɕۑ�
	SaveScene();
	// ���s��Ԃ�
	SetEngineMode(ENGINE_MODE::RUN);
	// �X�N���v�g��Start�֐����Ăяo��
	GetScene()->CallScriptStartFunc();
}

void Manager::StopScene()
{
	// �G�f�B�^�E�B���h�E�Ƀt�H�[�J�X��ݒ�
	ImGui::SetWindowFocus("Scene");
	// �G�f�B�^�őI�����Ă���I�u�W�F�N�g���N���A
	MyImGuiManager::GetInstance()->ClearSelectObject();
	// �V�[���ƃV�[�����X�g���N���A
	m_scene = nullptr;
	m_sceneList.clear();
	CollisionManager::GetInstance()->Uninit();
	LuaManager::GetInstance()->ClearScriptList();
	// �t�@�C������V�[�������[�h 
	m_editor = new Editor();
	m_editor->Init();
	LoadScene();
	// ���s�I��
	Manager::GetInstance()->SetEngineMode(ENGINE_MODE::EDIT);
}

void Manager::PauseScene()
{
}

bool Manager::CheckSceneFile()
{
	HANDLE h;
	WIN32_FIND_DATA win32FindData;
	std::string searchName = "Assets\\Scenes\\*";

	h = FindFirstFile(searchName.c_str(), &win32FindData);

	if (h == INVALID_HANDLE_VALUE) {
		return false;
	}
	return true;
}


std::list<std::string> Manager::GetAllFiles(std::string dirPath)
{
	HANDLE h;
	WIN32_FIND_DATA win32FindData;
	std::list<std::string> nameList;
	std::string searchName = dirPath + "\\*";

	h = FindFirstFile(searchName.c_str(), &win32FindData);

	if (h == INVALID_HANDLE_VALUE) {
		return nameList;
	}

	do {
		if (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// �擾�������̂��f�B���N�g���Ȃ疳��
		}
		else {
			nameList.push_back(win32FindData.cFileName);
		}
	} while (FindNextFile(h, &win32FindData));

	FindClose(h);

	return nameList;
}
