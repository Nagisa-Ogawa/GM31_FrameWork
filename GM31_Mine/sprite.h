#pragma once
#include <string>
#include "component.h"

class Sprite :public Component
{
private:
	D3DXVECTOR2 m_position;
	D3DXVECTOR2 m_size;
	std::string m_textureName;

	ID3D11Buffer* m_VertexBuffer = nullptr;
	ID3D11ShaderResourceView* m_Texture = nullptr;

public:
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size,const char* textureName);
	void Init(D3DXVECTOR2 pos, D3DXVECTOR2 size);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	D3DXVECTOR2* GetPosition() { return &m_position; }
	D3DXVECTOR2* GetSize() { return &m_size; }
	std::string GetTextureName() { return m_textureName; }

	void SetPosition(D3DXVECTOR2 pos) { m_position = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }
	void SetTextureName(std::string texName) { m_textureName = texName; }

};
