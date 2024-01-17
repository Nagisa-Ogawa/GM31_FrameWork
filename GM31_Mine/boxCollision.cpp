
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "CollisionManager.h"
#include "boxCollisionFrame.h"
#include "dispInspector.h"

void BoxCollision::Init(D3DXVECTOR3 size, D3DXVECTOR3 offset,bool isTrigger)
{
	m_Size = size;
	m_Offset = offset;
	m_IsTrigger = isTrigger;
	std::string name = m_gameObject->GetName() + "BoxCollisionFrame";
	auto m_CollFrame = Manager::GetInstance()->GetScene()->AddGameObject<BoxCollisionFrame>(1,name);
	m_CollFrame->Init(this);
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

void BoxCollision::DispInspector()
{
	DispComponent(this);
}

D3DXMATRIX* BoxCollision::GetWorldMatrix()
{
	// マトリクス設定
	D3DXVECTOR3 m_Scale = m_Size;
	D3DXVECTOR3 m_Rot = m_Transform->m_localRotation;
	D3DXVECTOR3 m_Pos = m_Transform->m_localPosition + m_Offset;
	D3DXMATRIX scale, rot, trans, world;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixTranslation(&trans, m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;
	if (m_Transform->m_parent != nullptr) {
		world = world * m_Transform->m_parent->m_worldMatrix;		
	}

	return &world;
}

