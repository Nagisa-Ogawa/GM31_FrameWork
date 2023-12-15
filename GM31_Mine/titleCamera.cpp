#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "titleCamera.h"
#include "player.h"
#include "cameraObject.h"


void TitleCamera::Init(D3DXVECTOR3 target)
{
	m_Target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

void TitleCamera::Uninit()
{
}

void TitleCamera::Update()
{

}


void TitleCamera::Draw()
{
	D3DXVECTOR3 position = m_gameObject->GetTransform()->m_position;
	// ビューマトリックス設定
	// D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVECTOR3 up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&m_ViewMatrix, &position, &m_Target, &up);

	Renderer::SetViewMatrix(&m_ViewMatrix);


	// プロジェクションマトリクス設定
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, 1.0f,
		(float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&projectionMatrix);
}

