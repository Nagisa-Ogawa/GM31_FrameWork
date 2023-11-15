#include "main.h"
#include "manager.h"
#include "scene.h"
#include "CollisionManager.h"
#include "sphereCollision.h"
#include "sphereCollisionFrame.h"

void SphereCollision::Init(float radius, D3DXVECTOR3 offset, bool isTrigger)
{
	m_Radius = radius;
	m_Offset = offset;

	auto m_CollFrame = Manager::GetInstance()->GetScene()->AddGameObject<SphereCollisionFrame>(1);
	m_CollFrame->Init(m_Radius, offset);
	m_CollFrame->SetCollTransform(m_GameObject->GetTransform());
	m_CollFrame->SetActive(false);

	CollisionManager::GetInstance()->AddSphereCollision(this);
	m_Transform = m_GameObject->GetTransform();
}

void SphereCollision::Uninit()
{
}

void SphereCollision::Update()
{
}

void SphereCollision::Draw()
{
}

D3DXMATRIX SphereCollision::GetWorldMatrix()
{
	// マトリクス設定
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(m_Radius, m_Radius, m_Radius);
	D3DXVECTOR3 m_Rot = m_Transform->m_rotation;
	D3DXVECTOR3 m_Pos = m_Transform->m_position + m_Offset;
	D3DXMATRIX scale, rot, trans, world;
	D3DXMatrixScaling(&scale, m_Radius, m_Radius, m_Radius);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixTranslation(&trans, m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;

	return world;
}
