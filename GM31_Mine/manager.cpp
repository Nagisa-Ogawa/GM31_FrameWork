#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"

Manager* Manager::m_Instance = NULL;

Manager::Manager()
{
	// なにもしない
}

Manager::Manager(const Manager& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
}

Manager& Manager::operator=(const Manager& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
	return *this;
}

Manager::~Manager()
{
	// インスタンスを解放
	delete m_Instance;
}

Manager* Manager::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
	if (m_Instance == NULL)
	{
		m_Instance = new Manager();
		m_Instance->Init();
	}
	return m_Instance;
}

void Manager::Init()
{
	Renderer::Init();
	m_Scene = new Scene();
	m_Scene->Init();
	Input::Init();
}

void Manager::Uninit()
{
	Input::Uninit();
	m_Scene->Uninit();
	delete m_Scene;
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();
}
