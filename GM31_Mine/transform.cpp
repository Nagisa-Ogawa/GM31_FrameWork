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
	// マトリクス設定
	D3DXMATRIX scale, rot, trans;
	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&trans, m_position.x, m_position.y, m_position.z);
	m_worldMatrix = scale * rot * trans;
	Renderer::SetWorldMatrix(&m_worldMatrix);
}

void Transform::SetRotationFromDegree(D3DXVECTOR3 deg)
{
	m_rotation.x = deg.x * (D3DX_PI / 180.0f);
	m_rotation.y = deg.y * (D3DX_PI / 180.0f);
	m_rotation.z = deg.z * (D3DX_PI / 180.0f);
}

D3DXVECTOR3 Transform::GetRotationAsDegree()
{
	D3DXVECTOR3 deg;
	deg.x = m_rotation.x * (180.0f / D3DX_PI);
	deg.y = m_rotation.y * (180.0f / D3DX_PI);
	deg.z = m_rotation.z * (180.0f / D3DX_PI);

	return deg;
}


