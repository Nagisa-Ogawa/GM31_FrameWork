#pragma once
#include "component.h"

class BoxCollisionFrame;

class BoxCollision :public Component
{
private:
	D3DXVECTOR3 m_Size{};
	D3DXVECTOR3 m_Offset{};
	bool		m_IsTrigger{};

	Transform* m_Transform{};

public:
	void Init(D3DXVECTOR3 size,D3DXVECTOR3 offset, bool isTrigger);
	void Uninit();
	void Update();
	void Draw();

	bool GetIsTrigger() { return m_IsTrigger; }
	D3DXVECTOR3 GetSize() { return m_Size; }
	D3DXVECTOR3 GetOffset() { return m_Offset; }
	D3DXMATRIX GetWorldMatrix();
};
