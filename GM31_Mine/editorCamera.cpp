#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "editorCamera.h"
#include "input.h"
#include "cameraObject.h"
#include "player.h"



void EditorCamera::Draw()
{
	
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


