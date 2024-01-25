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

	//  ゲームエンジンの状態に合わせて分岐
	switch (m_mode)
	{
	case EDIT:
		// 編集モードの時はエディタのみ更新処理を呼び出す
		m_editor->Update();
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

bool Manager::CheckSceneFile()
{
	return false;
}

/// <summary>
/// シーンをファイルに保存する関数
/// </summary>
/// <param name="fileName">保存するファイル名</param>
void Manager::SaveScene()
{
	// 現在あるシーンをすべて保存する
	for (auto scene : m_sceneList) {
		// Scenesフォルダにシーンファイルを作成
		std::string filePath = "Scenes\\"+scene->GetName() + ".json";
		std::ofstream file(filePath);
		// シーン情報をシリアライズ
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
/// シーンをロードする関数
/// </summary>
void Manager::LoadScene()
{
	// ファイルからシーンをロード
}
