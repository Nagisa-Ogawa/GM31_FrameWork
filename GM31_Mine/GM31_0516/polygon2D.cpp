
#include "main.h"
#include "renderer.h"
#include "polygon2D.h"
#include "sprite.h"


void Polygon2D::Init()
{

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout,
		"Shader/unlitTextureVS.cso");
	Renderer::CreatePixelShader(&m_PixelShader,
		"Shader/unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(D3DXVECTOR2(200.0f,200.0f), D3DXVECTOR2(200.0f, 200.0f), "asset/texture/Image20230424112827.jpg");
}

void Polygon2D::Uninit() 
{

	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

	GameObject::Uninit();
}

void Polygon2D::Update()
{
	GameObject::Update();
}


void Polygon2D::Draw()
{
	// 入力レイアウト設定
	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	// シェーダ設定
	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	// マトリクス設定
	Renderer::SetWorldViewProjection2D();

	GameObject::Draw();

}