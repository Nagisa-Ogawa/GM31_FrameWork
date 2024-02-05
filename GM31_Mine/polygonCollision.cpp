#include "polygonCollision.h"
#include "CollisionManager.h"

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
	delete[] m_polyArray;	// �z������
}

void PolygonCollision::Update()
{
}

void PolygonCollision::DispInspector()
{
}
