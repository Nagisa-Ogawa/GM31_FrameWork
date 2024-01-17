#pragma once

#include <string>
#include "component.h"


class Shader : public Component
{
private:
	std::string m_fileNameVS;
	std::string m_fileNamePS;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

public:
	void Init(const char* VSFileName,const char* PSFileName);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	std::string GetVSFile() { return m_fileNameVS; }
	std::string GetPSFile() { return m_fileNamePS; }
};
