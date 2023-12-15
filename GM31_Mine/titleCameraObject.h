#pragma once

#include "gameObject.h"

class TitleCameraObject :public GameObject
{
private:
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};
