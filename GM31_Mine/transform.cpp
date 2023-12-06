#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "scene.h"

#include "transform.h"


void Transform::Init(D3DXVECTOR3 position, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	m_position = position;
	m_rotation = rotation;
	m_scale = scale;
}

void Transform::Uninit()
{
}

void Transform::Update()
{
}

void Transform::Draw()
{
	//// マトリクス設定
	//D3DXMATRIX scale, rot, trans;
	//D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	//D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	//m_worldMatrix = scale * rot * trans;
	//Renderer::SetWorldMatrix(&m_worldMatrix);
}

void Transform::MakeWorldMatrix(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scale)
{
	// マトリクス設定
	D3DXMATRIX scaleMat, rotMat, transMat;
	D3DXMatrixScaling(&scaleMat, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rotMat, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&transMat, m_position.x, m_position.y, m_position.z);
	m_worldMatrix = scaleMat * rotMat * transMat;
	Renderer::SetWorldMatrix(&m_worldMatrix);
}

