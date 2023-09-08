#pragma once

#include "gameObject.h"

class Player:public GameObject
{

private:
	bool m_IsGround = false;
	bool m_IsHitEnemy = false;
	bool m_IsHitWall = false;
public:
	void Init() override;
	void Update() override;

	bool* GetIsHitEnemy() { return &m_IsHitEnemy; }
	bool* GetIsHitWall() { return &m_IsHitWall; }
};
