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

void Shader::Load()
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, m_fileNameVS.c_str());
	Renderer::CreatePixelShader(&m_PixelShader, m_fileNamePS.c_str());
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

void Shader::ChangeVertexShaer(std::string fileName)
{
	// 同じシェーダーファイルなら読み込まない
	if (fileName == m_fileNameVS) {
		return;
	}
	m_fileNameVS = fileName;
	if (m_VertexLayout) {
		m_VertexLayout->Release();
	}
	if (m_VertexShader) {
		m_VertexShader->Release();
	}
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, fileName.c_str());
}

void Shader::ChangePixelShaer(std::string fileName)
{
	// 同じシェーダーファイルなら読み込まない
	if (fileName == m_fileNamePS) {
		return;
	}
	m_fileNamePS = fileName;
	if (m_PixelShader) {
		m_PixelShader->Release();
	}
	Renderer::CreatePixelShader(&m_PixelShader, fileName.c_str());
}
