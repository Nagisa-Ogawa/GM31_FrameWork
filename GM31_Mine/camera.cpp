#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "input.h"
#include "cameraObject.h"
#include "player.h"
#include "dispInspector.h"


void Camera::Init()
{
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
	// カメラの行列にカメラがコンポーネントされているオブジェクトの行列をセット
	m_cameraMatrix = *(m_gameObject->GetTransform()->GetWorldMatrix());
	//// カメラ行列の逆行列からビュー変換行列を作成
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


