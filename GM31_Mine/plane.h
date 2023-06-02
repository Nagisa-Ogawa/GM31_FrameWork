#pragma once

#include "component.h"


class Plane :public Component
{
private:

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init(D3DXVECTOR3 pos,D3DXVECTOR2 size,const char* fileName);
	void Uninit();
	void Update();
	void Draw();
};
