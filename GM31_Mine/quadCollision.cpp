#include "main.h"
#include "manager.h"
#include "scene.h"
#include "quadCollision.h"
#include "CollisionManager.h"


void QuadCollision::Init(D3DXVECTOR3 normal, D3DXVECTOR2 size, bool isTrigger)
{
	m_Normal = normal;
	m_Size = size;
	m_IsTrigger = isTrigger;
	CollisionManager::GetInstance()->AddQuadCollision(this);
}

void QuadCollision::Uninit()
{
}

void QuadCollision::Update()
{
}

void QuadCollision::Draw()
{
}
