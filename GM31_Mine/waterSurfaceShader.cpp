#include "main.h"
#include "renderer.h"
#include "waterSurfaceShader.h"

void WaterSurfaceShader::Init(const char* VSFileName, const char* PSFileName)
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, VSFileName);
	Renderer::CreatePixelShader(&m_PixelShader, PSFileName);
	m_time = 0.0f;

}

void WaterSurfaceShader::Uninit()
{
	
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void WaterSurfaceShader::Update()
{
	m_time += 0.001f;
}

void WaterSurfaceShader::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);
	
	PARAMETER param;
	param.time = m_time;
	param.waveRate = 1.0f;
	param.waveLength = 0.1f;
	param.waveDirection = D3DXVECTOR2(0.4f, 0.7f);
	param.dropPos = D3DXVECTOR2(0.2f, 0.2f);
	param.dummy = 0.0f;

	Renderer::SetParameter(param);
	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

}
