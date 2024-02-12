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
	// なにもしない
}

Manager::Manager(const Manager& manager)
{
	// インスタンスをコピー
	m_instance = manager.m_instance;
}

Manager& Manager::operator=(const Manager& manager)
{
	// インスタンスをコピー
	m_instance = manager.m_instance;
	return *this;
}

Manager::~Manager()
{
	// インスタンスを解放
	delete m_instance;
}

Manager* Manager::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
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
	
	// Sceneファイルの中を確認
	if (CheckSceneFile()) {
		// Sceneのデータがあったなら読み込み処理
		LoadScene();
	}
	else {
		// Sceneのデータがないならシーンを作成
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

	//  ゲームエンジンの状態に合わせて分岐
	switch (m_mode)
	{
	case EDIT:
		// 編集モードの時はエディタのみ更新処理を呼び出す
		m_editor->Update();
		// Luaファイルが更新されているかチェック
		LuaManager::GetInstance()->CheckUpdateScript();
		break;
	case RUN:
		// 実行中の時はエディタとゲームの更新処理をどちらも呼び出す
		m_editor->Update();
		Input::Update();
		m_scene->Update();

		break;
	case PAUSE:
		// 一時停止中はエディタのみ更新処置を呼び出す
		m_editor->Update();
		break;
	default:
		break;
	}

	// GUIの更新処理
	MyImGuiManager::GetInstance()->Update();

	// オブジェクトが破棄されているかをチェック
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
	// テクスチャにエディタ画面をレンダリング
	Renderer::EditorViewBegin();

	m_editor->Draw();
	m_scene->Draw();

	// テクスチャに実行画面をレンダリング
	Renderer::GameViewBegin();

	m_scene->Draw();

	// 画面全体をレンダリング
	Renderer::Begin();

	MyImGuiManager::GetInstance()->Draw();

	Renderer::End();

}


/// <summary>
/// シーンをファイルに保存する関数
/// </summary>
/// <param name="fileName">保存するファイル名</param>
void Manager::SaveScene()
{
	// 現在あるシーンをすべて保存する
	for (const auto& scene : m_sceneList) {
		// Scenesフォルダにシーンファイルを作成
		std::string filePath = "Assets\\Scenes\\"+scene->GetName() + ".json";
		std::ofstream file(filePath);
		// シーン情報をシリアライズ
		try {
			cereal::JSONOutputArchive archive(file);
			archive(scene);
		}
		catch (std::exception& e) {
			MyImGuiManager::GetInstance()->DebugLog(e.what());
		}
	}
	// 終了したら成功したことを通知
	MyImGuiManager::GetInstance()->DebugLog("Save Successful !!");
}

/// <summary>
/// シーンをロードする関数
/// </summary>
void Manager::LoadScene()
{
	// ファイルからシーンをロード
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
		// ファイルから読み込んだ後に読み込み後関数を呼び出し
		m_scene = scene.get();
		m_editor = scene->GetEditor();
		scene->Load();
		m_sceneList.push_back(std::move(scene));
	}
	// 終了したら成功したことを通知
	MyImGuiManager::GetInstance()->DebugLog("Load Successful !!");
}

// シーンを再生する関数
void Manager::PlayScene()
{
	// ゲームウィンドウにフォーカスを設定
	ImGui::SetWindowFocus("Game");
	// 現在のシーンをファイルに保存
	SaveScene();
	// 実行状態へ
	SetEngineMode(ENGINE_MODE::RUN);
	// スクリプトのStart関数を呼び出す
	GetScene()->CallScriptStartFunc();
}

void Manager::StopScene()
{
	// エディタウィンドウにフォーカスを設定
	ImGui::SetWindowFocus("Scene");
	// エディタで選択しているオブジェクトをクリア
	MyImGuiManager::GetInstance()->ClearSelectObject();
	// シーンとシーンリストをクリア
	m_scene = nullptr;
	m_sceneList.clear();
	CollisionManager::GetInstance()->Uninit();
	LuaManager::GetInstance()->ClearScriptList();
	// ファイルからシーンをロード 
	m_editor = new Editor();
	m_editor->Init();
	LoadScene();
	// 実行終了
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
			// 取得したものがディレクトリなら無視
		}
		else {
			nameList.push_back(win32FindData.cFileName);
		}
	} while (FindNextFile(h, &win32FindData));

	FindClose(h);

	return nameList;
}
