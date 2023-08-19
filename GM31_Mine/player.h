#pragma once

#include "gameObject.h"

class Player:public GameObject
{

private:
	bool m_IsGround = false;
public:
	void Init() override;
	void Update() override;
};
