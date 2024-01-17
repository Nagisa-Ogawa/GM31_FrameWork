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
	// �J�����̑���
	EditorCamera* camera = GetComponent<EditorCamera>();
	if (ImGui::IsMouseDown(ImGuiMouseButton_Right)&&
		MyImGuiManager::GetInstance()->GetFocusWindow()==ImGui::FindWindowByName("Scene")) {
		// ���̃t���[���ŏ��߂ĉE�N���b�N�������Ƃ�
		if (!m_isRButton) {
			// �t���O��ON�ɂ��A���W���L�^
			m_startMousePos = Input::GetClientMousePos();
			m_isRButton = true;
		}
		// �E�N���b�N���Ȃ���WASD�L�[�ŕ��s�ړ�
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
		// �E�N���b�N���Ȃ���QE�L�[�ŏ㉺�ړ�
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
	// �}�E�X�z�C�[���ŃY�[���C���E�A�E�g
	// if(Input::GetKeyPress(vk_m))

	if (m_isRButton) {
		POINT nowMousePos = Input::GetClientMousePos();
		// �}�E�X�̈ړ��ʂ��v�Z
		D3DXVECTOR2 mouseDistance;
		mouseDistance.x = (nowMousePos.x - m_startMousePos.x) * MOUSE_SENSITIVITY;
		mouseDistance.y = (nowMousePos.y - m_startMousePos.y) * MOUSE_SENSITIVITY;
		// �J��������]������
		RotateCamera(mouseDistance);
		// �}�E�X�̊J�n�ʒu���X�V
		m_startMousePos = nowMousePos;
	}
}


/// <summary>
/// �J�������J�������g�𒆐S�ɉ�]������֐�
/// </summary>
/// <param name="delta">��]��������Ɨ�( x : y���̉�]��  y : x���̉�]��)</param>
void EditorCameraObject::RotateCamera(D3DXVECTOR2 delta)
{
	EditorCamera* camera = GetComponent<EditorCamera>();
	m_transform->m_localRotation.x += delta.y;
	m_transform->m_localRotation.y += delta.x;
	// ��]�ʂ��J�����֔��f
	camera->SetRotation(m_transform->m_localRotation);

}
