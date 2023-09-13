#pragma once

#include "gameObject.h"

class BoxCollisionFrame : public GameObject
{

private:
	class Transform* m_collTransform{};
	D3DXVECTOR3 m_Offset{};
public:
	void Init(D3DXVECTOR3 scale,D3DXVECTOR3 offset);
	void Update() override;

	void SetCollTransform(Transform* transform) { m_collTransform = transform; }
};