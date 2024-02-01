#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "cameraObject.h"
#include "player.h"
#include "dispInspector.h"


void Camera::Init(D3DXVECTOR3 position)
{
	m_position = position;
	
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}

void Camera::Load()
{
}

void Camera::Uninit()
{
}

void Camera::Update()
{
	
}


void Camera::Draw()
{
	// エディタに描画するときはゲームのカメラは行列を設定しない
	if (Renderer::GetRenderTarget() != RENDER_TARGET::GAME) {
		return;
	}
	// D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &m_target, &m_up);
	// カメラの移動と回転行列をからカメラ行列を作成(拡大は使わない)
	D3DXMATRIX rot, trans;
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	m_cameraMatrix = rot * trans;
	// カメラ行列の逆行列からビュー変換行列を作成
	D3DXMatrixInverse(&m_viewMatrix, NULL, &m_cameraMatrix);

	Renderer::SetViewMatrix(&m_viewMatrix);


	// プロジェクションマトリクス設定
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f,
		(float)GAMESCREEN_WIDTH / GAMESCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_projectionMatrix);
}

void Camera::DispInspector()
{
	DispComponent(this);
}


