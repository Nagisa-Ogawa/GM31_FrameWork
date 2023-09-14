#pragma once
#include "component.h"

class SphereCollisionFrame;

class SphereCollision : public Component
{
private:
	float		m_Radius{};
	D3DXVECTOR3 m_Offset{};
	bool		m_IsTrigger{};

	Transform* m_Transform{};

public:
	void Init(float radius, D3DXVECTOR3 offset, bool isTrigger);
	void Uninit();
	void Update();
	void Draw();

	bool GetIsTrigger() { return m_IsTrigger; }
	float GetRadius() { return m_Radius; }
	D3DXVECTOR3 GetOffset() { return m_Offset; }
	D3DXMATRIX GetWorldMatrix();
};
