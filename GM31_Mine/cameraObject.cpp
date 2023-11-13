#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "cameraObject.h"
#include "input.h"

#define MOUSE_SENSITIVITY (0.002);

void CameraObject::Init()
{
	m_transform->m_Position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_transform->m_Rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<Camera>()->Init(m_transform->m_Position);
}

void CameraObject::Update()
{
	// カメラの操作
	Camera* camera = GetComponent<Camera>();
	D3DXVECTOR3 target = camera->GetTarget();
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
			m_transform->m_Position -= m_transform->GetRight() * 0.1f;
			target -= m_transform->GetRight() * 0.1f;
		}
		if (Input::GetKeyPress('D'))
		{
			m_transform->m_Position += m_transform->GetRight() * 0.1f;
			target += m_transform->GetRight() * 0.1f;
		}
		if (Input::GetKeyPress('S'))
		{
			m_transform->m_Position -= m_transform->GetForward() * 0.1f;
			target -= m_transform->GetForward() * 0.1f;
		}
		if (Input::GetKeyPress('W'))
		{
			m_transform->m_Position += m_transform->GetForward() * 0.1f;
			target += m_transform->GetForward() * 0.1f;
		}
		// 右クリックしながらQEキーで上下移動
		if (Input::GetKeyPress('Q'))
		{
			m_transform->m_Position -= m_transform->GetUp() * 0.1f;
			target -= m_transform->GetUp() * 0.1f;
		}
		if (Input::GetKeyPress('E'))
		{
			m_transform->m_Position += m_transform->GetUp() * 0.1f;
			target += m_transform->GetUp() * 0.1f;
		}
	}
	else {
		m_isRButton = false;
	}

	camera->SetPosition(m_transform->m_Position);
	camera->SetTarget(target);
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
	m_transform->m_Rotation.x += delta.x;
	m_transform->m_Rotation.y += delta.y;
	// 引数から回転行列を作成
	D3DXMATRIX rotateYMatrix;
	// xmatrixro(&rotateMatrix, delta.x, delta.y, 0.0f);

	// 回転行列をカメラの各要素にかけ合わせる
	//D3DXVec3TransformCoord(&m_transform->m_Position, &m_transform->m_Position, &rotateMatrix);
	//camera->SetPosition(m_transform->m_Position);

	//D3DXVECTOR3 target = camera->GetTarget();
	//D3DXVec3TransformCoord(&target, &target, &rotateMatrix);
	//camera->SetTarget(target);
	//
	//D3DXVECTOR3 up = camera->GetUp();
	//D3DXVec3TransformCoord(&up, &up, &rotateMatrix);
	//camera->SetUp(up);
}
