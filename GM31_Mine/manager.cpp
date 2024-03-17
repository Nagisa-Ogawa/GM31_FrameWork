#include <fstream>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"

#include "main.h"
#include "manager.h"
#include "myImGuiManager.h"
#include "luaManager.h"
#include "collisionManager.h"
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

	Renderer::Init();		// �`��֌W������
	LuaManager::GetInstance()->Init();	// Lua�֌W������
	MyImGuiManager::GetInstance()->Init(GetWindow());	// ImGui�֌W������
	
	// Scene�t�@�C���̒����m�F
	if (!CheckSceneFile()) {
		// Scene�̃f�[�^���������Ȃ�ǂݍ��ݏ���
		LoadEditScene();
	}
	else {
		// Scene�̃f�[�^���Ȃ��Ȃ�V�[�����쐬
		m_scene = std::make_unique<Scene>();
		m_scene->SetName("SampleScene");
		m_scene->Init();
		m_editor = m_scene->GetEditor();
		SaveEditScene();
	}
}


void Manager::Uninit()
{
	SaveEditScene();
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

	// �V�[�����ύX����邩�`�F�b�N
	CheckChangeScene();
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

void Manager::CheckChangeScene()
{
	if (m_nextScene)
	{
		// �V�[��������Ȃ�I���������Ă�
		if (m_scene)
		{
			m_scene->Uninit();
		}
		// GUI�Ō��ݎQ�Ƃ��Ă���I�u�W�F�N�g���N���A
		MyImGuiManager::GetInstance()->ClearSelectObject();
		// �|�C���^�����
		m_scene.reset();
		// �V�����V�[������
		m_scene = std::move(m_nextScene);
		m_editor = m_scene->GetEditor();
		// �V�[���̃��[�h�������Ăяo��
		m_scene->Load();
		if (m_mode == ENGINE_MODE::RUN) {
			// �X�N���v�g��Start�֐����Ăяo��
			GetScene()->CallScriptStartFunc();
		}
		m_nextScene = nullptr;
	}
}

/// <summary>
/// �V�[�����쐬����֐�
/// </summary>
/// <param name="sceneName">�쐬����V�[����</param>
bool Manager::CreateScene(std::string sceneName)
{
	// �V�����V�[�����쐬
	std::unique_ptr<Scene> scene= std::make_unique<Scene>();
	scene->SetName(sceneName);
	scene->Init();
	// �V�[�����t�@�C���ɕۑ�
	if (SaveScene(scene)) {
		MyImGuiManager::GetInstance()->DebugLog("Successful!! create scene");
		return true;
	}
	return false;
}

/// <summary>
/// ���݂̃V�[����ύX����֐�
/// </summary>
/// <param name="sceneName">�ύX����V�[����</param>
bool Manager::ChangeScene(std::string sceneName)
{
	if (!IsExistScene(sceneName)) {
		MyImGuiManager::GetInstance()->DebugLog("Faild!! Can't change scene. this scene name isn't used.");
		return false;
	}
	// �Q�[�����łȂ��Ȃ�V�[����ς���O�ɃZ�[�u
	if (m_mode == ENGINE_MODE::EDIT) {
		Manager::GetInstance()->SaveEditScene();
	}
	// �V�[�������[�h
	if (LoadScene(m_nextScene, sceneName)) {
		return true;
	}
	else {
		return false;
	}
}


void Manager::SaveEditScene()
{
	// ���݂̃V�[���̖��O��ۑ�
	SaveEditorSceneName();
	// ���݂̃V�[����ۑ�
	SaveScene(m_scene);
}

/// <summary>
/// ���݂̃V�[�������[�h����֐�
/// </summary>
void Manager::LoadEditScene()
{
	std::string fileName = LoadEditorSceneName();
	LoadScene(m_scene,fileName);
	// �t�@�C������ǂݍ��񂾌�ɓǂݍ��݌�֐����Ăяo��
	m_editor = m_scene->GetEditor();
	m_scene->Load();
}


/// <summary>
/// �V�[�����t�@�C���ɕۑ�����֐�
/// </summary>
/// <param name="fileName">�ۑ�����t�@�C����</param>
bool Manager::SaveScene(std::unique_ptr<Scene>& scene)
{
	// ���݂̃V�[����ۑ�����
	// Scenes�t�H���_�ɃV�[���t�@�C�����쐬
	std::string filePath = "Assets\\Scenes\\"+ scene->GetName() + ".json";
	std::ofstream file(filePath);
	if (file.fail()) {
		return false;
	}
	// �V�[�������V���A���C�Y
	try {
		cereal::JSONOutputArchive archive(file);
		archive(scene);
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
		return false;
	}
	// �I�������琬���������Ƃ�ʒm
	MyImGuiManager::GetInstance()->DebugLog("Save Successful !!");
	return true;
}


/// <summary>
/// �V�[�������[�h����֐�
/// </summary>
bool Manager::LoadScene(std::unique_ptr<Scene>& scene, std::string sceneName)
{
	std::string fileName = sceneName;
	if (sceneName == "") {
		return false;
	}
	fileName += ".json";
	// �ҏW���Ă����V�[���t�@�C�������[�h
	try {
		std::ifstream file("Assets\\Scenes\\" + fileName);
		cereal::JSONInputArchive archive(file);
		archive(scene);
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
		return false;
	}
	// �I�������琬���������Ƃ�ʒm
	MyImGuiManager::GetInstance()->DebugLog("Load Successful !!");
	return true;
}

/// <summary>
/// �ҏW���Ă����V�[������ۑ�����֐�
/// </summary>
void Manager::SaveEditorSceneName()
{
	std::string filePath = "Assets\\Scenes\\EditSceneInfo\\sceneName.json";
	std::ofstream file(filePath);
	// �V�[�������V���A���C�Y
	try {
		cereal::JSONOutputArchive archive(file);
		archive(cereal::make_nvp("EditSceneName",m_scene->GetName()));
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
	}
	// �I�������琬���������Ƃ�ʒm
	MyImGuiManager::GetInstance()->DebugLog("Successful!! Save EditSceneName");

}


/// <summary>
/// �ҏW���Ă����V�[���������[�h����֐�
/// </summary>
/// <returns>�ҏW���Ă����V�[����</returns>
std::string Manager::LoadEditorSceneName()
{
	std::string fileName = "Assets\\Scenes\\EditSceneInfo\\sceneName.json";
	std::ifstream file(fileName);
	if (file.fail()) {
		MyImGuiManager::GetInstance()->DebugLog("Faild!! "+fileName+" is can't find");
		return "";
	}
	std::string editSceneName;
	try {
		cereal::JSONInputArchive archive(file);
		archive(editSceneName);
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
		return "";
	}
	// �I�������琬���������Ƃ�ʒm
	MyImGuiManager::GetInstance()->DebugLog("Successful!! Load EditSceneName");
	return editSceneName;
}

// �V�[�����Đ�����֐�
void Manager::PlayScene()
{
	// �Q�[���E�B���h�E�Ƀt�H�[�J�X��ݒ�
	ImGui::SetWindowFocus("Game");
	// ���݂̃V�[�����t�@�C���ɕۑ�
	SaveEditScene();
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
	// �V�[�����N���A
	m_scene.reset();
	m_scene = nullptr;
	CollisionManager::GetInstance()->Uninit();
	LuaManager::GetInstance()->ClearScriptList();
	// �t�@�C������V�[�������[�h 
	LoadEditScene();
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

	do {
		if (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// �擾�������̂��f�B���N�g���Ȃ疳��
		}
		else {
			// �t�@�C������ł��������Ȃ�o��
			return true;
		}
	} while (FindNextFile(h, &win32FindData));

	FindClose(h);

	return false;
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

/// <summary>
/// �����̃V�[�����̃V�[���t�@�C�������݂��邩�`�F�b�N����֐�
/// </summary>
/// <param name="name">�T���V�[���̖��O</param>
/// <returns>���݂������ǂ���</returns>
bool Manager::IsExistScene(std::string name)
{
	name += ".json";
	HANDLE h;
	WIN32_FIND_DATA win32FindData;
	std::list<std::string> nameList;
	std::string searchName = "Assets\\Scenes\\*";

	h = FindFirstFile(searchName.c_str(), &win32FindData);

	if (h == INVALID_HANDLE_VALUE) {
		return false;
	}

	do {
		if (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// �擾�������̂��f�B���N�g���Ȃ疳��
		}
		else {
			if (win32FindData.cFileName == name) {
				return true;
			}
		}
	} while (FindNextFile(h, &win32FindData));

	FindClose(h);

	return false;
}
