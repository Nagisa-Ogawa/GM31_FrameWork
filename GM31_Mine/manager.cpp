#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
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
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;
	Renderer::Uninit();
}

void Manager::Update()
{
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();
}
