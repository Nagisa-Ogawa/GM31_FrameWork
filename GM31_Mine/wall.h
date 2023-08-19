#pragma once

#include "gameObject.h"

class Wall : public GameObject
{

private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
