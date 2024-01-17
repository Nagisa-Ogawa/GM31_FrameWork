#pragma once
#include "component.h"

class SphereCollisionFrame;

class SphereCollision : public Component
{
private:
	float		m_radius{};
	D3DXVECTOR3 m_offset{};
	bool		m_isTrigger{};

	Transform* m_transform{};

public:
	void Init(float radius, D3DXVECTOR3 offset, bool isTrigger);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	bool* GetIsTrigger() { return &m_isTrigger; }
	float* GetRadius() { return &m_radius; }
	D3DXVECTOR3* GetOffset() { return &m_offset; }
	D3DXMATRIX GetWorldMatrix();

	void SetIsTrigger(bool flag) { m_isTrigger = flag; }
	void SetRadius(float r) { m_radius = r; }
	void SetOffset(D3DXVECTOR3 offset) { m_offset = offset; }
};
