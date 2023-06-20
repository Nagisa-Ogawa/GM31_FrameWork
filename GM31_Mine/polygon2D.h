#pragma once

#include "gameObject.h"

class Polygon2D:public GameObject
{

private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};
