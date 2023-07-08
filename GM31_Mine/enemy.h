#pragma once

#include "gameObject.h"

struct MODEL;

class Enemy : public GameObject
{

private:
	MODEL* m_pModel = nullptr;

public:

	void Init(MODEL* pModel);
	void Update() override;

};
