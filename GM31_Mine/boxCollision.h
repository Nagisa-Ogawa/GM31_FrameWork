#pragma once
#include "component.h"


class BoxCollision :public Component
{
private:
	D3DXVECTOR3 m_Size = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	D3DXVECTOR3 m_Offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool	m_IsTrigger = false;
public:
	void Init(D3DXVECTOR3 size,D3DXVECTOR3 offset, bool isTrigger);
	void Uninit();
	void Update();
	void Draw();

	bool GetIsTrigger() { return m_IsTrigger; }
	D3DXVECTOR3* GetSize() { return &m_Size; }
};
