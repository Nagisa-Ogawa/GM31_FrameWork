#include "polygonCollision.h"
#include "collisionManager.h"

void PolygonCollision::Init()
{
	CollisionManager::GetInstance()->AddPolygonCollision(this);
}

void PolygonCollision::Load()
{
	CollisionManager::GetInstance()->AddPolygonCollision(this);
}

void PolygonCollision::Uninit()
{
	CollisionManager::GetInstance()->DeletePolygonCollision(this);
	delete[] m_polyArray;	// �z������
}

void PolygonCollision::Update()
{
}

void PolygonCollision::DispInspector()
{
}
