#pragma once

#include "gameObject.h"

class Player:public GameObject
{

private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
