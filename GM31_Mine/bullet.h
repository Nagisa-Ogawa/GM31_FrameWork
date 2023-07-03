#pragma once

#include "gameObject.h"

class Bullet : public GameObject
{

private:
	D3DXVECTOR3 m_Direction{};
	bool m_IsUse = false;
public:
	void SetUse(bool flag) { m_IsUse = flag; }
	bool GetUse() { return m_IsUse; }

	void Init() override;
	void Update() override;

};
