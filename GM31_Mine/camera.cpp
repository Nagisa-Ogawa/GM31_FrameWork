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
	// �G�f�B�^�ɕ`�悷��Ƃ��̓Q�[���̃J�����͍s���ݒ肵�Ȃ�
	if (Renderer::GetRenderTarget() != RENDER_TARGET::GAME) {
		return;
	}
	// �J�����̍s��ɃJ�������R���|�[�l���g����Ă���I�u�W�F�N�g�̍s����Z�b�g
	m_cameraMatrix = *(m_gameObject->GetTransform()->GetWorldMatrix());
	//// �J�����s��̋t�s�񂩂�r���[�ϊ��s����쐬
	D3DXMatrixInverse(&m_viewMatrix, NULL, &m_cameraMatrix);

	Renderer::SetViewMatrix(&m_viewMatrix);


	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f,
		(float)GAMESCREEN_WIDTH / GAMESCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_projectionMatrix);
}

void Camera::DispInspector()
{
	DispComponent(this);
}


