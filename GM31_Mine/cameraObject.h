#pragma once

#include "gameObject.h"

class CameraObject:public GameObject
{
private:
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	
};
