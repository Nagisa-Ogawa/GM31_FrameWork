#include "main.h"
#include "renderer.h"
#include "shader.h"

void Shader::Init(const char * VSFileName, const char * PSFileName)
{
	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"Shader\\vertexLightingVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"Shader\\vertexLightingPS.cso");

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
	// ���̓��C�A�E�g�ݒ�
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// �V�F�[�_�ݒ�
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

}
