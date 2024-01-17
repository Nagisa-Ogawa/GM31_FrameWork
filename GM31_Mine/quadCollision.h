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
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	bool* GetIsTrigger() { return &m_IsTrigger; }
	D3DXVECTOR3* GetNormal() { return &m_Normal; }
	D3DXVECTOR2* GetSize() { return &m_Size; }

	void SetIsTrigger(bool flag) { m_IsTrigger = flag; }
	void SetNormal(D3DXVECTOR3 n) { m_Normal = n; }
	void SetSize(D3DXVECTOR2 s) { m_Size = s; }

};
