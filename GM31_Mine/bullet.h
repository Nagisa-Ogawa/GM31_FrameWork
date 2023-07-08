#pragma once

#include "gameObject.h"

struct MODEL;

class Bullet : public GameObject
{

private:
	MODEL* m_pModel = nullptr;
	D3DXVECTOR3 m_StartPos{};
	D3DXVECTOR3 m_Direction{};
	float m_Speed = 0.0f;
public:
	void SetStartPos(D3DXVECTOR3 pos) { m_StartPos = pos; }

	void Init(MODEL* pModel);
	void Update() override;

	void SetDirection(D3DXVECTOR3 direction) { m_Direction = direction; }

};
