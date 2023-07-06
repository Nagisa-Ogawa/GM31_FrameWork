#pragma once

#include "gameObject.h"

class MODEL;

class Bullet : public GameObject
{

private:
	D3DXVECTOR3 m_Direction{};
	MODEL* m_pModel = nullptr;
public:
	void SetModel(MODEL* pModel) { m_pModel = pModel; }

	void Init(D3DXVECTOR3 direction) override;
	void Update() override;


};
