#pragma once

#include "gameObject.h"
#include "model.h"

class Sky :public GameObject
{

private:

public:
	void Init() override;
	void Update() override;
};
