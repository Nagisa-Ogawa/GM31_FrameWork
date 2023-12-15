#pragma once

#include "gameObject.h"
#include "model.h"

class Ball :public GameObject
{

private:
	MODEL* m_pModel = nullptr;
	D3DXVECTOR2 m_velocity{};	// ‘¬“x
	float m_m{};	// Ž¿—Ê

public:
	void Init(MODEL* pModel,D3DXVECTOR3 pos, D3DXVECTOR2 vel, float m);
	void Update() override;

	D3DXVECTOR2 GetVelocity() { return m_velocity; }
	float		GetMass() { return m_m; }
	void		SetVelocity(D3DXVECTOR2 vel) { m_velocity = vel; }
};
