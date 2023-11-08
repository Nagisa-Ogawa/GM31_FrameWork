#pragma once

#include "gameObject.h"

class Polygon2D:public GameObject
{

private:

public:
	void Init(D3DXVECTOR2 pos,D3DXVECTOR2 size,const char* textureName);
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Uninit();
	void Update();
	void Draw();
};
