#pragma once

#include "gameObject.h"

class Player:public GameObject
{

private:

public:
	void Init() override;
	void Update() override;
};
