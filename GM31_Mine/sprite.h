#pragma once
#include "component.h"

class Sprite :public Component
{
private:
	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size,const char* textureName);
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Uninit();
	void Update();
	void Draw();

};
