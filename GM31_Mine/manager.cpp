#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "scene.h"
#include "input.h"
#include "title.h"
#include "game.h"

Manager* Manager::m_Instance = NULL;

Manager::Manager()
{
	// �Ȃɂ����Ȃ�
}

Manager::Manager(const Manager& manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
}

Manager& Manager::operator=(const Manager& manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
	return *this;
}

Manager::~Manager()
{
	// �C���X�^���X�����
	delete m_Instance;
}

Manager* Manager::GetInstance()
{
	// ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
	// ����ȍ~�͐��������C���X�^���X��n��
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
	SetScene<Title>();
	Input::Init();
}

void Manager::Uninit()
{
	m_Scene->Uninit();
	delete m_Scene;
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (m_NextScene) 
	{
		if (m_Scene)
		{
			m_Scene->Uninit();
			delete m_Scene;
		}
		m_Scene = m_NextScene;
		m_Scene->Init();
		m_NextScene = nullptr;
	}
	m_Scene->Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();
	MyImGuiManager::GetInstance()->Draw();

	Renderer::End();
}
