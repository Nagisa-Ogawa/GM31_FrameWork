
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "CollisionManager.h"
#include "boxCollisionFrame.h"
#include "dispInspector.h"


void BoxCollision::Init()
{
	m_size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_isTrigger = false;
	std::string name = m_gameObject->GetName() + "BoxCollisionFrame";
	m_collFrame = Manager::GetInstance()->GetScene()->GetEditor()->AddEditorObject<BoxCollisionFrame>(1, name);
	m_collFrame->Init(this);
	m_collID = m_collFrame->GetID();

	CollisionManager::GetInstance()->AddBoxCollision(this);
}

void BoxCollision::Load()
{
	m_collFrame = Manager::GetInstance()->GetScene()->GetEditor()->GetEditorObjectWithID<BoxCollisionFrame>(m_collID);
	m_collFrame->SetCollision(this);
	CollisionManager::GetInstance()->AddBoxCollision(this);
}

void BoxCollision::Uninit()
{
	// 当たり判定マネージャーのリストから削除
	CollisionManager::GetInstance()->DeleteBoxCollision(this);
	// 当たり判定表示用オブジェクトを削除
	m_collFrame->SetDestroy();
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
	D3DXMatrixScaling(&scale, m_size.x, m_size.y, m_size.z);
	D3DXMatrixTranslation(&trans, m_offset.x, m_offset.y, m_offset.z);
	world = scale * trans;
	auto transform = m_gameObject->GetTransform();
	D3DXMATRIX worldRot = transform->GetWorldRotMatrix();
	D3DXMATRIX worldTrans = transform->GetWorldTransMatrix();
	m_worldMatrix = world * (worldRot * worldTrans);

	return &m_worldMatrix;
}

