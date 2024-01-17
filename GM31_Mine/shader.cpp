#include "main.h"
#include "renderer.h"
#include "shader.h"
#include "dispInspector.h"

void Shader::Init(const char* VSFileName, const char* PSFileName)
{
	m_fileNameVS = VSFileName;
	m_fileNamePS = PSFileName;
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,VSFileName);
	Renderer::CreatePixelShader(&m_PixelShader,PSFileName);
}

void Shader::Uninit()
{
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Shader::Update()
{
}

void Shader::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

}

void Shader::DispInspector()
{
	DispComponent(this);
}
