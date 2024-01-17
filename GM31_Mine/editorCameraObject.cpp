#include "main.h"
#include "manager.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "editorCamera.h"
#include "editorCameraObject.h"
#include "input.h"

#define MOUSE_SENSITIVITY (0.003);
#define MOVECAMERA_SPEED  (0.2)

void EditorCameraObject::Init()
{
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_transform->m_localRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<EditorCamera>()->Init(m_transform->m_localPosition);
}

void EditorCameraObject::Update()
{
	// カメラの操作
	EditorCamera* camera = GetComponent<EditorCamera>();
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right)&&
		MyImGuiManager::GetInstance()->GetFocusWindow()==ImGui::FindWindowByName("Scene")) {
		// このフレームで初めて右クリックをしたとき
		if (!m_isRButton) {
			// フラグをONにし、座標を記録
			m_startMousePos = Input::GetClientMousePos();
			m_isRButton = true;
		}
		// 右クリックしながらWASDキーで平行移動
		if (ImGui::IsKeyDown(ImGuiKey_A))
		{
			m_transform->m_localPosition -= m_transform->GetRight() * MOVECAMERA_SPEED;
		}
		if (ImGui::IsKeyDown(ImGuiKey_D))
		{
			m_transform->m_localPosition += m_transform->GetRight() * MOVECAMERA_SPEED;
		}
		if (ImGui::IsKeyDown(ImGuiKey_S))
		{
			m_transform->m_localPosition -= m_transform->GetForward() * MOVECAMERA_SPEED;
		}
		if (ImGui::IsKeyDown(ImGuiKey_W))
		{
			m_transform->m_localPosition += m_transform->GetForward() * MOVECAMERA_SPEED;
		}
		// 右クリックしながらQEキーで上下移動
		if (ImGui::IsKeyDown(ImGuiKey_Q))
		{
			m_transform->m_localPosition -= m_transform->GetUp() * MOVECAMERA_SPEED;
		}
		if (ImGui::IsKeyDown(ImGuiKey_E))
		{
			m_transform->m_localPosition += m_transform->GetUp() * MOVECAMERA_SPEED;
		}
	}
	else {
		m_isRButton = false;
	}

	camera->SetPosition(m_transform->m_localPosition);
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
void EditorCameraObject::RotateCamera(D3DXVECTOR2 delta)
{
	EditorCamera* camera = GetComponent<EditorCamera>();
	m_transform->m_localRotation.x += delta.y;
	m_transform->m_localRotation.y += delta.x;
	// 回転量をカメラへ反映
	camera->SetRotation(m_transform->m_localRotation);

}
