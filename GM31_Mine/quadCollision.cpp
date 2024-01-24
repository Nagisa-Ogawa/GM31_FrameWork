#include "main.h"
#include "manager.h"
#include "scene.h"
#include "quadCollision.h"
#include "CollisionManager.h"
#include "dispInspector.h"


void QuadCollision::Init(D3DXVECTOR3 normal, D3DXVECTOR2 size, bool isTrigger)
{
	m_normal = normal;
	m_size = size;
	m_isTrigger = isTrigger;
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

void QuadCollision::DispInspector()
{
	DispComponent(this);
}
