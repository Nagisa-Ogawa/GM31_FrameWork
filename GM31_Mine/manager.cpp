#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "LuaManager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "game.h"
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
