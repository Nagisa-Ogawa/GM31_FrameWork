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
	
	// �J�����̈ړ��Ɖ�]�s�������J�����s����쐬(�g��͎g��Ȃ�)
	D3DXMATRIX rot, trans;
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	m_cameraMatrix = rot * trans;
	// �J�����s��̋t�s�񂩂�r���[�ϊ��s����쐬
	D3DXMatrixInverse(&m_viewMatrix, NULL, &m_cameraMatrix);

	Renderer::SetViewMatrix(&m_viewMatrix);


	// �v���W�F�N�V�����}�g���N�X�ݒ�
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, 1.0f,
		(float)GAMESCREEN_WIDTH / GAMESCREEN_HEIGHT, 1.0f, 1000.0f);

	Renderer::SetProjectionMatrix(&m_projectionMatrix);
}


