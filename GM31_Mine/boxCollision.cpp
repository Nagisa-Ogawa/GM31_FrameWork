
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "CollisionManager.h"

void BoxCollision::Init(D3DXVECTOR3 size, D3DXVECTOR3 offset,bool isTrigger)
{
	m_Size = size;
	m_Offset = offset;
	m_IsTrigger = isTrigger;
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
