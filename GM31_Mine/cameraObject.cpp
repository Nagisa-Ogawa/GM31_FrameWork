#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "cameraObject.h"

void CameraObject::Init()
{
	m_Transform->m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	AddComponent<Camera>();
}

void CameraObject::Uninit()
{
	GameObject::Uninit();
}

void CameraObject::Update()
{
	GameObject::Update();
}

void CameraObject::Draw()
{
	GameObject::Draw();
}
