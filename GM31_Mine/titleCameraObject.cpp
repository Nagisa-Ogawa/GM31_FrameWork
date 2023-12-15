#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "titleCameraObject.h"
#include "titleCamera.h"

void TitleCameraObject::Init()
{
	m_transform->m_position = D3DXVECTOR3(0.0f, 16.0f, -15.0f);
	AddComponent<TitleCamera>();
}

void TitleCameraObject::Uninit()
{
	GameObject::Uninit();
}

void TitleCameraObject::Update()
{
	GameObject::Update();
}

void TitleCameraObject::Draw()
{
	GameObject::Draw();
}
