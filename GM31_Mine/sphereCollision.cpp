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
	m_IsShowFrame = true;
	m_CollFrame = Manager::GetInstance()->GetScene()->AddGameObject<SphereCollisionFrame>(1);
	m_CollFrame->Init(radius,offset);
	m_CollFrame->SetCollTransform(m_GameObject->GetTransform());
	if (!m_IsShowFrame) {
		m_CollFrame->SetActive(false);
	}


	CollisionManager::GetInstance()->AddSphereCollision(this);

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

void SphereCollision::SetIsShowFrame(bool flag)
{
	m_IsShowFrame = flag;
	m_CollFrame->SetActive(flag);
}
