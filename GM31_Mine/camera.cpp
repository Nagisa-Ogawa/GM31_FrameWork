#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "cameraObject.h"
#include "player.h"


void Camera::Init(D3DXVECTOR3 position)
{
	m_position = position;
	m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	
}


void Camera::Draw()
{
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_target, &m_up);

	Renderer::SetViewMatrix(&m_viewMatrix);


	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f,
		(float)GAMESCREEN_WIDTH / GAMESCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_projectionMatrix);
}


