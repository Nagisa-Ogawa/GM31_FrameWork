#include "main.h"
#include "manager.h"
#include "scene.h"
#include "collisionManager.h"
#include "sphereCollision.h"
#include "sphereCollisionFrame.h"
#include "dispInspector.h"


void SphereCollision::Init()
{
	m_radius = 1.0f;
	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	std::string name = m_gameObject->GetName() + "SphereCollisionFram";
	auto m_CollFrame = Manager::GetInstance()->GetScene()->GetEditor()->AddEditorObject<SphereCollisionFrame>(1, name);
	m_CollFrame->Init(m_radius, m_offset);
	m_CollFrame->SetCollTransform(m_gameObject->GetTransform());
	m_CollFrame->SetActive(false);

	CollisionManager::GetInstance()->AddSphereCollision(this);
	m_transform = m_gameObject->GetTransform();
}

void SphereCollision::Load()
{
	std::string name = m_gameObject->GetName() + "SphereCollisionFram";
	auto m_CollFrame = Manager::GetInstance()->GetScene()->GetEditor()->AddEditorObject<SphereCollisionFrame>(1, name);
	m_CollFrame->Init(m_radius, m_offset);
	m_CollFrame->SetCollTransform(m_gameObject->GetTransform());
	m_CollFrame->SetActive(false);

	CollisionManager::GetInstance()->AddSphereCollision(this);
	m_transform = m_gameObject->GetTransform();
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
	// �}�g���N�X�ݒ�
	D3DXVECTOR3 m_Scale = D3DXVECTOR3(m_radius, m_radius, m_radius);
	D3DXQUATERNION m_qua = m_transform->m_localQuaternion;
	D3DXVECTOR3 m_Pos = m_transform->m_localPosition + m_offset;
	D3DXMATRIX scale, rot, trans, world;
	D3DXMatrixScaling(&scale, m_radius, m_radius, m_radius);
	D3DXMatrixRotationQuaternion(&rot, &m_qua);
	D3DXMatrixTranslation(&trans, m_Pos.x, m_Pos.y, m_Pos.z);
	world = scale * rot * trans;

	return world;
}
