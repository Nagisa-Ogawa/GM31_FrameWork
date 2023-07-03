#pragma once

#include "gameObject.h"

class Field : public GameObject
{

private:

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
