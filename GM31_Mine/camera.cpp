#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "player.h"
#include "cameraObject.h"


void Camera::Init(D3DXVECTOR3 target)
{
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	Scene* scene = Manager::GetInstance()->GetScene();
	Player* player = scene->GetGameObject<Player>();

	//// 見下ろし型
	//m_Target = player->GetPosition();
	//m_Position = m_Target + D3DXVECTOR3(0.0f, 5.0f, -10.0f);

	//// 三人称視点
	m_Target = player->GetTransform()->m_Position + player->GetTransform()->GetRight()*1.0f + D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_GameObject->GetTransform()->m_Position = m_Target - player->GetTransform()->GetForward()*8.0f + D3DXVECTOR3(0.0f, 4.0f, 0.0f);

	// 一人称視点
	//m_Target = m_GameObject->GetTransform()->m_Position+ player->GetTransform()->GetForward();
	//m_GameObject->GetTransform()->m_Position = player->GetTransform()->m_Position + D3DXVECTOR3(0.0f, 1.5f, 0.0f);

	// ハクスラ風
	//m_Position = player->GetPosition() + D3DXVECTOR3(0.0f, 15.0f, 0.0f);
	//m_Target = player->GetPosition();
}


void Camera::Draw()
{
	D3DXVECTOR3 position = m_GameObject->GetTransform()->m_Position;
	// ビューマトリックス設定
	// D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_ProjectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_ProjectionMatrix);
}

