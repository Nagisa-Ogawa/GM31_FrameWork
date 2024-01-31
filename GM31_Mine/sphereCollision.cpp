#include "main.h"
#include "manager.h"
#include "scene.h"
#include "CollisionManager.h"
#include "sphereCollision.h"
#include "sphereCollisionFrame.h"
#include "dispInspector.h"

void SphereCollision::Init(float radius, D3DXVECTOR3 offset, bool isTrigger)
{
	m_radius = radius;
	m_offset = offset;
	std::string name = m_gameObject->GetName() + "SphereCollisionFram";
	auto m_CollFrame = Manager::GetInstance()->GetScene()->AddGameObject<SphereCollisionFrame>(1,name);
	m_CollFrame->Init(m_radius, offset);
	m_CollFrame->SetCollTransform(m_gameObject->GetTransform().get());
	m_CollFrame->SetActive(false);

	CollisionManager::GetInstance()->AddSphereCollision(this);
	m_transform = m_gameObject->GetTransform().get();
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

void SphereCollision::DispInspector()
{
	DispComponent(this);
}

D3DXMATRIX SphereCollision::GetWorldMatrix()
{
	// マトリクス設定
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(m_radius, m_radius, m_radius);
	D3DXVECTOR3 m_Rot = m_transform->m_localRotation;
	D3DXVECTOR3 m_Pos = m_transform->m_localPosition + m_offset;
	D3DXMATRIX scale, rot, trans, world;
	D3DXMatrixScaling(&scale, m_radius, m_radius, m_radius);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixTranslation(&trans, m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;

	return world;
}
