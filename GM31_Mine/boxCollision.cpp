
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
	auto m_CollFrame = Manager::GetInstance()->GetEditor()->AddGameObject<BoxCollisionFrame>(1,name);
	m_CollFrame->Init(this);
	// m_CollFrame->SetActive(false);

	CollisionManager::GetInstance()->AddBoxCollision(this);
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
	D3DXMATRIX scale, trans, world;
	D3DXMatrixScaling(&scale, m_Size.x, m_Size.y, m_Size.z);
	D3DXMatrixTranslation(&trans, m_Offset.x, m_Offset.y, m_Offset.z);
	world = scale * trans;
	Transform* transform = m_gameObject->GetTransform();
	D3DXMATRIX worldRot = transform->GetWorldRotMatrix();
	D3DXMATRIX worldTrans = transform->GetWorldTransMatrix();
	m_worldMatrix = world * (worldRot * worldTrans);

	return &m_worldMatrix;
}

