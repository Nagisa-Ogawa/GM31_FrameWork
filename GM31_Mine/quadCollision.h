#pragma once
#include "component.h"


class QuadCollision :public Component
{
private:
	D3DXVECTOR3 m_Normal{};
	D3DXVECTOR2 m_Size{};
	bool	m_IsTrigger = false;
public:
	void Init(D3DXVECTOR3 normal, D3DXVECTOR2 size, bool isTrigger);
	void Uninit();
	void Update();
	void Draw();

	bool GetIsTrigger() { return m_IsTrigger; }
	D3DXVECTOR3* GetNormal() { return &m_Normal; }
	D3DXVECTOR2* GetSize() { return &m_Size; }
};
