
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "CollisionManager.h"
#include "boxCollisionFrame.h"

void BoxCollision::Init(D3DXVECTOR3 size, D3DXVECTOR3 offset,bool isTrigger)
{
	m_Size = size;
	m_Offset = offset;
	m_IsTrigger = isTrigger;
	std::string name = m_gameObject->GetName() + "BoxCollisionFrame";
	auto m_CollFrame = Manager::GetInstance()->GetScene()->AddGameObject<BoxCollisionFrame>(1,name);
	m_CollFrame->Init(size,offset);
	m_CollFrame->SetCollTransform(m_gameObject->GetTransform());
	m_CollFrame->SetActive(false);

	CollisionManager::GetInstance()->AddBoxCollision(this);
	m_Transform = m_gameObject->GetTransform();
}

void BoxCollision::Uninit()
{
}

void BoxCollision::Update()
{
}

void BoxCollision::Draw()
{
}

D3DXMATRIX BoxCollision::GetWorldMatrix()
{
	// マトリクス設定
	D3DXVECTOR3 m_Scale = m_Size;
	D3DXVECTOR3 m_Rot = m_Transform->m_rotation;
	D3DXVECTOR3 m_Pos = m_Transform->m_position + m_Offset;
	D3DXMATRIX scale, rot, trans, world;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixTranslation(&trans, m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;

	return world;
}

