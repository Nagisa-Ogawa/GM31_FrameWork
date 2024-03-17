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

	Renderer::Init();		// 描画関係初期化
	LuaManager::GetInstance()->Init();	// Lua関係初期化
	MyImGuiManager::GetInstance()->Init(GetWindow());	// ImGui関係初期化
	
	// Sceneファイルの中を確認
	if (!CheckSceneFile()) {
		// Sceneのデータがあったなら読み込み処理
		LoadEditScene();
	}
	else {
		// Sceneのデータがないならシーンを作成
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

	// シーンが変更されるかチェック
	CheckChangeScene();
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

void Manager::CheckChangeScene()
{
	if (m_nextScene)
	{
		// シーンがあるなら終了処理を呼ぶ
		if (m_scene)
		{
			m_scene->Uninit();
		}
		// GUIで現在参照しているオブジェクトをクリア
		MyImGuiManager::GetInstance()->ClearSelectObject();
		// ポインタを解放
		m_scene.reset();
		// 新しいシーンを代入
		m_scene = std::move(m_nextScene);
		m_editor = m_scene->GetEditor();
		// シーンのロード処理を呼び出す
		m_scene->Load();
		if (m_mode == ENGINE_MODE::RUN) {
			// スクリプトのStart関数を呼び出す
			GetScene()->CallScriptStartFunc();
		}
		m_nextScene = nullptr;
	}
}

/// <summary>
/// シーンを作成する関数
/// </summary>
/// <param name="sceneName">作成するシーン名</param>
bool Manager::CreateScene(std::string sceneName)
{
	// 新しいシーンを作成
	std::unique_ptr<Scene> scene= std::make_unique<Scene>();
	scene->SetName(sceneName);
	scene->Init();
	// シーンをファイルに保存
	if (SaveScene(scene)) {
		MyImGuiManager::GetInstance()->DebugLog("Successful!! create scene");
		return true;
	}
	return false;
}

/// <summary>
/// 現在のシーンを変更する関数
/// </summary>
/// <param name="sceneName">変更するシーン名</param>
bool Manager::ChangeScene(std::string sceneName)
{
	if (!IsExistScene(sceneName)) {
		MyImGuiManager::GetInstance()->DebugLog("Faild!! Can't change scene. this scene name isn't used.");
		return false;
	}
	// ゲーム中でないならシーンを変える前にセーブ
	if (m_mode == ENGINE_MODE::EDIT) {
		Manager::GetInstance()->SaveEditScene();
	}
	// シーンをロード
	if (LoadScene(m_nextScene, sceneName)) {
		return true;
	}
	else {
		return false;
	}
}


void Manager::SaveEditScene()
{
	// 現在のシーンの名前を保存
	SaveEditorSceneName();
	// 現在のシーンを保存
	SaveScene(m_scene);
}

/// <summary>
/// 現在のシーンをロードする関数
/// </summary>
void Manager::LoadEditScene()
{
	std::string fileName = LoadEditorSceneName();
	LoadScene(m_scene,fileName);
	// ファイルから読み込んだ後に読み込み後関数を呼び出し
	m_editor = m_scene->GetEditor();
	m_scene->Load();
}


/// <summary>
/// シーンをファイルに保存する関数
/// </summary>
/// <param name="fileName">保存するファイル名</param>
bool Manager::SaveScene(std::unique_ptr<Scene>& scene)
{
	// 現在のシーンを保存する
	// Scenesフォルダにシーンファイルを作成
	std::string filePath = "Assets\\Scenes\\"+ scene->GetName() + ".json";
	std::ofstream file(filePath);
	if (file.fail()) {
		return false;
	}
	// シーン情報をシリアライズ
	try {
		cereal::JSONOutputArchive archive(file);
		archive(scene);
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
		return false;
	}
	// 終了したら成功したことを通知
	MyImGuiManager::GetInstance()->DebugLog("Save Successful !!");
	return true;
}


/// <summary>
/// シーンをロードする関数
/// </summary>
bool Manager::LoadScene(std::unique_ptr<Scene>& scene, std::string sceneName)
{
	std::string fileName = sceneName;
	if (sceneName == "") {
		return false;
	}
	fileName += ".json";
	// 編集していたシーンファイルをロード
	try {
		std::ifstream file("Assets\\Scenes\\" + fileName);
		cereal::JSONInputArchive archive(file);
		archive(scene);
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
		return false;
	}
	// 終了したら成功したことを通知
	MyImGuiManager::GetInstance()->DebugLog("Load Successful !!");
	return true;
}

/// <summary>
/// 編集していたシーン名を保存する関数
/// </summary>
void Manager::SaveEditorSceneName()
{
	std::string filePath = "Assets\\Scenes\\EditSceneInfo\\sceneName.json";
	std::ofstream file(filePath);
	// シーン情報をシリアライズ
	try {
		cereal::JSONOutputArchive archive(file);
		archive(cereal::make_nvp("EditSceneName",m_scene->GetName()));
	}
	catch (std::exception& e) {
		MyImGuiManager::GetInstance()->DebugLog(e.what());
	}
	// 終了したら成功したことを通知
	MyImGuiManager::GetInstance()->DebugLog("Successful!! Save EditSceneName");

}


/// <summary>
/// 編集していたシーン名をロードする関数
/// </summary>
/// <returns>編集していたシーン名</returns>
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
	// 終了したら成功したことを通知
	MyImGuiManager::GetInstance()->DebugLog("Successful!! Load EditSceneName");
	return editSceneName;
}

// シーンを再生する関数
void Manager::PlayScene()
{
	// ゲームウィンドウにフォーカスを設定
	ImGui::SetWindowFocus("Game");
	// 現在のシーンをファイルに保存
	SaveEditScene();
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
	// シーンをクリア
	m_scene.reset();
	m_scene = nullptr;
	CollisionManager::GetInstance()->Uninit();
	LuaManager::GetInstance()->ClearScriptList();
	// ファイルからシーンをロード 
	LoadEditScene();
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

	do {
		if (win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			// 取得したものがディレクトリなら無視
		}
		else {
			// ファイルが一つでもあったなら出る
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
			// 取得したものがディレクトリなら無視
		}
		else {
			nameList.push_back(win32FindData.cFileName);
		}
	} while (FindNextFile(h, &win32FindData));

	FindClose(h);

	return nameList;
}

/// <summary>
/// 引数のシーン名のシーンファイルが存在するかチェックする関数
/// </summary>
/// <param name="name">探すシーンの名前</param>
/// <returns>存在したかどうか</returns>
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
			// 取得したものがディレクトリなら無視
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
