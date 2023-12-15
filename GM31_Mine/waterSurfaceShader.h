#pragma once

#include "component.h"

class WaterSurfaceShader : public Component
{
private:
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

	float m_time;
public:
	void Init(const char* VSFileName, const char* PSFileName);
	void Uninit();
	void Update();
	void Draw();

};
