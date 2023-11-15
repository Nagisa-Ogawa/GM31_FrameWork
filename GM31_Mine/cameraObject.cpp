#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "cameraObject.h"
#include "input.h"

#define MOUSE_SENSITIVITY (0.003);
#define MOVECAMERA_SPEED  (0.2)

void CameraObject::Init()
{
	m_transform->m_position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_transform->m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<Camera>()->Init(m_transform->m_position);
}

void CameraObject::Update()
{
	// カメラの操作
	Camera* camera = GetComponent<Camera>();
	if (Input::GetKeyPress(VK_RBUTTON)) {
		// このフレームで初めて右クリックをしたとき
		if (!m_isRButton) {
			// フラグをONにし、座標を記録
			m_startMousePos = Input::GetClientMousePos();
			m_isRButton = true;
		}
		// 右クリックしながらWASDキーで平行移動
		if (Input::GetKeyPress('A'))
		{
			m_transform->m_position -= m_transform->GetRight() * MOVECAMERA_SPEED;
		}
		if (Input::GetKeyPress('D'))
		{
			m_transform->m_position += m_transform->GetRight() * MOVECAMERA_SPEED;
		}
		if (Input::GetKeyPress('S'))
		{
			m_transform->m_position -= m_transform->GetForward() * MOVECAMERA_SPEED;
		}
		if (Input::GetKeyPress('W'))
		{
			m_transform->m_position += m_transform->GetForward() * MOVECAMERA_SPEED;
		}
		// 右クリックしながらQEキーで上下移動
		if (Input::GetKeyPress('Q'))
		{
			m_transform->m_position -= m_transform->GetUp() * MOVECAMERA_SPEED;
		}
		if (Input::GetKeyPress('E'))
		{
			m_transform->m_position += m_transform->GetUp() * MOVECAMERA_SPEED;
		}
	}
	else {
		m_isRButton = false;
	}

	camera->SetPosition(m_transform->m_position);
	// マウスホイールでズームイン・アウト
	// if(Input::GetKeyPress(vk_m))

	if (m_isRButton) {
		POINT nowMousePos = Input::GetClientMousePos();
		// マウスの移動量を計算
		D3DXVECTOR2 mouseDistance;
		mouseDistance.x = (nowMousePos.x - m_startMousePos.x) * MOUSE_SENSITIVITY;
		mouseDistance.y = (nowMousePos.y - m_startMousePos.y) * MOUSE_SENSITIVITY;
		// カメラを回転させる
		RotateCamera(mouseDistance);
		// マウスの開始位置を更新
		m_startMousePos = nowMousePos;
	}
}


/// <summary>
/// カメラをカメラ自身を中心に回転させる関数
/// </summary>
/// <param name="delta">回転する方向と量( x : y軸の回転量  y : x軸の回転量)</param>
void CameraObject::RotateCamera(D3DXVECTOR2 delta)
{
	Camera* camera = GetComponent<Camera>();
	m_transform->m_rotation.x += delta.y;
	m_transform->m_rotation.y += delta.x;
	// 回転量をカメラへ反映
	camera->SetRotation(m_transform->m_rotation);
	
}
