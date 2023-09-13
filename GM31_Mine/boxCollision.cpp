
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

	m_CollFrame = Manager::GetInstance()->GetScene()->AddGameObject<BoxCollisionFrame>(1);
	m_CollFrame->Init(size,offset);
	m_CollFrame->SetCollTransform(m_GameObject->GetTransform());
	if (!m_IsShowFrame) {
		m_CollFrame->SetActive(false);
	}


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

void BoxCollision::SetIsShowFrame(bool flag)
{
	m_IsShowFrame = flag;
	m_CollFrame->SetActive(flag);
}
