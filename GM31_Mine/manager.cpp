#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
//GameObject* g_Polygon = nullptr;
//GameObject* g_Camera = nullptr;
//GameObject* g_Field = nullptr;
//GameObject* g_Player = nullptr;

Scene* Manager::m_Scene{};

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
