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
	// �J�����̑���
	Camera* camera = GetComponent<Camera>();
	if (Input::GetKeyPress(VK_RBUTTON)) {
		// ���̃t���[���ŏ��߂ĉE�N���b�N�������Ƃ�
		if (!m_isRButton) {
			// �t���O��ON�ɂ��A���W���L�^
			m_startMousePos = Input::GetClientMousePos();
			m_isRButton = true;
		}
		// �E�N���b�N���Ȃ���WASD�L�[�ŕ��s�ړ�
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
		// �E�N���b�N���Ȃ���QE�L�[�ŏ㉺�ړ�
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
void CameraObject::RotateCamera(D3DXVECTOR2 delta)
{
	Camera* camera = GetComponent<Camera>();
	m_transform->m_rotation.x += delta.y;
	m_transform->m_rotation.y += delta.x;
	// ��]�ʂ��J�����֔��f
	camera->SetRotation(m_transform->m_rotation);
	
}
