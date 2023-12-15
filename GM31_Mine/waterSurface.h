#pragma once

#include "gameObject.h"
#include "model.h"

class WaterSurface :public GameObject
{

private:

public:
	void Init() override;
	void Update() override;
};
