#pragma once

#include "gameObject.h"

class QuadCollision;

class Wall : public GameObject
{

private:
public:
	void Init(D3DXVECTOR3 pos,D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();

};
