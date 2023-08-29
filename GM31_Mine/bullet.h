#pragma once

#include "gameObject.h"

struct MODEL;

class Bullet : public GameObject
{

private:
	MODEL* m_pModel = nullptr;
	D3DXVECTOR3 m_Direction{};
	float m_Speed = 0.0f;

	D3DXVECTOR3 m_StartVec{};
	D3DXVECTOR3 m_EndVec{};
	D3DXVECTOR3 m_StartPos{};
	D3DXVECTOR3 m_EndPos{};
	int m_ThrowFrame = 120;
	int m_NowFrame = 0;
	D3DXVECTOR3 m_pVec{};
public:
	void SetStartVec(D3DXVECTOR3 vec) { m_StartPos = vec; }
	void SetPlayerVec(D3DXVECTOR3 vec) { m_pVec = vec; }

	void Init(MODEL* pModel);
	void Update() override;

	void SetDirection(D3DXVECTOR3 direction) { m_Direction = direction; }
	D3DXVECTOR3 Throw();
	void Set();
};