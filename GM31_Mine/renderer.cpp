
#include "main.h"
#include "renderer.h"
#include <io.h>

//#define _CRT_SECURE_NO_WARNINGS

RENDER_TARGET			Renderer::m_renderTarget = RENDER_TARGET::ENGINE;

D3D_FEATURE_LEVEL       Renderer::m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::m_Device{};
ID3D11DeviceContext*    Renderer::m_DeviceContext{};
IDXGISwapChain*         Renderer::m_SwapChain{};
ID3D11RenderTargetView* Renderer::m_RenderTargetView{};
ID3D11DepthStencilView* Renderer::m_DepthStencilView{};

ID3D11Buffer*			Renderer::m_WorldBuffer{};
ID3D11Buffer*			Renderer::m_ViewBuffer{};
ID3D11Buffer*			Renderer::m_ProjectionBuffer{};
ID3D11Buffer*			Renderer::m_MaterialBuffer{};
ID3D11Buffer*			Renderer::m_LightBuffer{};
ID3D11Buffer*			Renderer::m_ParameterBuffer{};


ID3D11DepthStencilState* Renderer::m_DepthStateEnable{};
ID3D11DepthStencilState* Renderer::m_DepthStateDisable{};


ID3D11BlendState*		Renderer::m_BlendState{};
ID3D11BlendState*		Renderer::m_BlendStateATC{};

// エディタビュー用テクスチャ
ID3D11Texture2D*			Renderer::m_editorViewTexture{};
ID3D11RenderTargetView*		Renderer::m_editorViewRenderTargetView{};
ID3D11ShaderResourceView* Renderer::m_editorViewShaderresourceView{};
ID3D11DepthStencilView*	Renderer::m_editorViewDepthStencilView{};
// ゲームビュー用テクスチャ
ID3D11Texture2D* Renderer::m_gameViewTexture{};
ID3D11RenderTargetView* Renderer::m_gameViewRenderTargetView{};
ID3D11ShaderResourceView* Renderer::m_gameViewShaderresourceView{};
ID3D11DepthStencilView* Renderer::m_gameViewDepthStencilView{};


UINT Renderer::m_ResizeWidth{};
UINT Renderer::m_ResizeHeight{};

void Renderer::Init()
{
	HRESULT hr = S_OK;




	// デバイス、スワップチェーン作成
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = SCREEN_WIDTH;
	swapChainDesc.BufferDesc.Height = SCREEN_HEIGHT;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = GetWindow();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain( NULL,
										D3D_DRIVER_TYPE_HARDWARE,
										NULL,
										0,
										NULL,
										0,
										D3D11_SDK_VERSION,
										&swapChainDesc,
										&m_SwapChain,
										&m_Device,
										&m_FeatureLevel,
										&m_DeviceContext );



	SetRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT);

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
	rasterizerDesc.CullMode = D3D11_CULL_BACK; 
	rasterizerDesc.DepthClipEnable = TRUE; 
	rasterizerDesc.MultisampleEnable = FALSE; 

	ID3D11RasterizerState *rs;
	m_Device->CreateRasterizerState( &rasterizerDesc, &rs );

	m_DeviceContext->RSSetState( rs );




	// ブレンドステート設定
	D3D11_BLEND_DESC blendDesc{};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	m_Device->CreateBlendState( &blendDesc, &m_BlendState );

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_Device->CreateBlendState( &blendDesc, &m_BlendStateATC );

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff );





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_Device->CreateDepthStencilState( &depthStencilDesc, &m_DepthStateDisable );//深度無効ステート

	m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	m_Device->CreateSamplerState( &samplerDesc, &samplerState );

	m_DeviceContext->PSSetSamplers( 0, 1, &samplerState );



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_WorldBuffer );
	m_DeviceContext->VSSetConstantBuffers( 0, 1, &m_WorldBuffer);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ViewBuffer );
	m_DeviceContext->VSSetConstantBuffers( 1, 1, &m_ViewBuffer );

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_ProjectionBuffer );
	m_DeviceContext->VSSetConstantBuffers( 2, 1, &m_ProjectionBuffer );


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_MaterialBuffer );
	m_DeviceContext->VSSetConstantBuffers( 3, 1, &m_MaterialBuffer );
	m_DeviceContext->PSSetConstantBuffers( 3, 1, &m_MaterialBuffer );


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_Device->CreateBuffer( &bufferDesc, NULL, &m_LightBuffer );
	m_DeviceContext->VSSetConstantBuffers( 4, 1, &m_LightBuffer );
	m_DeviceContext->PSSetConstantBuffers( 4, 1, &m_LightBuffer );

	bufferDesc.ByteWidth = sizeof(PARAMETER);

	m_Device->CreateBuffer(&bufferDesc, nullptr, &m_ParameterBuffer);
	m_DeviceContext->VSSetConstantBuffers(5, 1, &m_ParameterBuffer);
	m_DeviceContext->PSSetConstantBuffers(5, 1, &m_ParameterBuffer);



	// ライト初期化
	LIGHT light{};
	light.Enable = true;
	light.Direction = D3DXVECTOR4(0.5f, -1.0f, 0.8f, 0.0f);
	D3DXVec4Normalize(&light.Direction, &light.Direction);
	light.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light.Diffuse = D3DXCOLOR(1.5f, 1.5f, 1.5f, 1.0f);
	SetLight(light);



	// マテリアル初期化
	MATERIAL material{};
	material.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);


	// ------------------------------------
	// エディタビュー用のテクスチャの設定
	// ------------------------------------
	D3D11_TEXTURE2D_DESC editorVTD;
	D3D11_RENDER_TARGET_VIEW_DESC editorRTVD;
	D3D11_SHADER_RESOURCE_VIEW_DESC editorSRVD;

	// テクスチャの設定
	ZeroMemory(&editorVTD, sizeof(editorVTD));
	editorVTD.Width = GAMESCREEN_WIDTH;
	editorVTD.Height = GAMESCREEN_HEIGHT;
	editorVTD.MipLevels = 1;
	editorVTD.ArraySize = 1;
	editorVTD.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	editorVTD.SampleDesc.Count = 1;
	editorVTD.Usage = D3D11_USAGE_DEFAULT;
	editorVTD.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	editorVTD.CPUAccessFlags = 0;
	editorVTD.MiscFlags = 0;
	editorVTD.SampleDesc.Count = 1;
	editorVTD.SampleDesc.Quality = 0;

	// テクスチャの作成
	m_Device->CreateTexture2D(&editorVTD, NULL, &m_editorViewTexture);


	// レンダーターゲットの設定
	editorRTVD.Format = editorVTD.Format;
	editorRTVD.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	editorRTVD.Texture2D.MipSlice = 0;

	// レンダーターゲットの作成
	m_Device->CreateRenderTargetView(m_editorViewTexture, &editorRTVD, &m_editorViewRenderTargetView);
	

	// シェーダーリソースビューの設定
	editorSRVD.Format = editorVTD.Format;
	editorSRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	editorSRVD.Texture2D.MostDetailedMip = 0;
	editorSRVD.Texture1D.MipLevels = 1;

	// シェーダーリソースビューの作成
	hr= m_Device->CreateShaderResourceView(m_editorViewTexture, &editorSRVD, &m_editorViewShaderresourceView);


	// デプスステンシルバッファ作成
	D3D11_TEXTURE2D_DESC editorTD;
	m_editorViewTexture->GetDesc(&editorTD);
	editorTD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	editorTD.Usage = D3D11_USAGE_DEFAULT;
	editorTD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_Device->CreateTexture2D(&editorTD, NULL, &m_editorViewTexture);
	m_Device->CreateDepthStencilView(m_editorViewTexture, NULL, &m_editorViewDepthStencilView);



	// -------------------------------------------
	// ゲームビュー用のテクスチャの設定
	// -------------------------------------------
	D3D11_TEXTURE2D_DESC gameVTD;
	D3D11_RENDER_TARGET_VIEW_DESC gameRTVD;
	D3D11_SHADER_RESOURCE_VIEW_DESC gameSRVD;

	// テクスチャの設定
	ZeroMemory(&gameVTD, sizeof(gameVTD));
	gameVTD.Width = GAMESCREEN_WIDTH;
	gameVTD.Height = GAMESCREEN_HEIGHT;
	gameVTD.MipLevels = 1;
	gameVTD.ArraySize = 1;
	gameVTD.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	gameVTD.SampleDesc.Count = 1;
	gameVTD.Usage = D3D11_USAGE_DEFAULT;
	gameVTD.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	gameVTD.CPUAccessFlags = 0;
	gameVTD.MiscFlags = 0;
	gameVTD.SampleDesc.Count = 1;
	gameVTD.SampleDesc.Quality = 0;

	// テクスチャの作成
	m_Device->CreateTexture2D(&gameVTD, NULL, &m_gameViewTexture);


	// レンダーターゲットの設定
	gameRTVD.Format = gameVTD.Format;
	gameRTVD.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	gameRTVD.Texture2D.MipSlice = 0;

	// レンダーターゲットの作成
	m_Device->CreateRenderTargetView(m_gameViewTexture, &gameRTVD, &m_gameViewRenderTargetView);


	// シェーダーリソースビューの設定
	gameSRVD.Format = gameVTD.Format;
	gameSRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	gameSRVD.Texture2D.MostDetailedMip = 0;
	gameSRVD.Texture1D.MipLevels = 1;

	// シェーダーリソースビューの作成
	hr = m_Device->CreateShaderResourceView(m_gameViewTexture, &gameSRVD, &m_gameViewShaderresourceView);


	// デプスステンシルバッファ作成
	D3D11_TEXTURE2D_DESC gameTD;
	m_gameViewTexture->GetDesc(&gameTD);
	gameTD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	gameTD.Usage = D3D11_USAGE_DEFAULT;
	gameTD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_Device->CreateTexture2D(&gameTD, NULL, &m_gameViewTexture);
	m_Device->CreateDepthStencilView(m_gameViewTexture, NULL, &m_gameViewDepthStencilView);

}



void Renderer::Uninit()
{

	m_WorldBuffer->Release();
	m_ViewBuffer->Release();
	m_ProjectionBuffer->Release();
	m_LightBuffer->Release();
	m_MaterialBuffer->Release();


	m_DeviceContext->ClearState();
	m_RenderTargetView->Release();
	m_SwapChain->Release();
	m_DeviceContext->Release();
	m_Device->Release();

	m_editorViewTexture->Release();
	m_editorViewRenderTargetView->Release();
	m_editorViewDepthStencilView->Release();
	m_editorViewShaderresourceView->Release();

	m_gameViewTexture->Release();
	m_gameViewRenderTargetView->Release();
	m_gameViewDepthStencilView->Release();
	m_gameViewShaderresourceView->Release();
}




void Renderer::Begin()
{
	m_renderTarget = RENDER_TARGET::ENGINE;
	SetViewport(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_DeviceContext->ClearRenderTargetView( m_RenderTargetView, clearColor );
	m_DeviceContext->ClearDepthStencilView( m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::EditorViewBegin()
{
	m_renderTarget = RENDER_TARGET::EDITOR;
	// ビューポートを変更
	SetViewport(GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT);
	m_DeviceContext->OMSetRenderTargets(1, &m_editorViewRenderTargetView, m_editorViewDepthStencilView);
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_editorViewRenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_editorViewDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::GameViewBegin()
{
	m_renderTarget = RENDER_TARGET::GAME;
	// ビューポートを変更
	SetViewport(GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT);
	m_DeviceContext->OMSetRenderTargets(1, &m_gameViewRenderTargetView, m_gameViewDepthStencilView);
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_DeviceContext->ClearRenderTargetView(m_gameViewRenderTargetView, clearColor);
	m_DeviceContext->ClearDepthStencilView(m_gameViewDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}




void Renderer::End()
{
	m_SwapChain->Present( 1, 0 );
}


void Renderer::SetRenderTarget(UINT width, UINT height)
{
	ID3D11Texture2D* renderTarget{};
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_Device->CreateRenderTargetView(renderTarget, NULL, &m_RenderTargetView);
	renderTarget->Release();

	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_Device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	m_Device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_DepthStencilView);
	depthStencile->Release();

	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

	SetViewport(width, height);
}

void Renderer::CleanRenderTarget()
{
	if (m_RenderTargetView) { m_RenderTargetView->Release(); m_RenderTargetView = nullptr; }
	if (m_DepthStencilView) { m_DepthStencilView->Release(); m_DepthStencilView = nullptr; }
}


void Renderer::SetDepthEnable( bool Enable )
{
	if( Enable )
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateEnable, NULL );
	else
		m_DeviceContext->OMSetDepthStencilState( m_DepthStateDisable, NULL );

}



void Renderer::SetATCEnable( bool Enable )
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (Enable)
		m_DeviceContext->OMSetBlendState(m_BlendStateATC, blendFactor, 0xffffffff);
	else
		m_DeviceContext->OMSetBlendState(m_BlendState, blendFactor, 0xffffffff);

}

void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_DeviceContext->UpdateSubresource( m_ProjectionBuffer, 0, NULL, &projection, 0, 0 );

}


void Renderer::SetWorldMatrix( D3DXMATRIX* WorldMatrix )
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, WorldMatrix);
	m_DeviceContext->UpdateSubresource(m_WorldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix( D3DXMATRIX* ViewMatrix )
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, ViewMatrix);
	m_DeviceContext->UpdateSubresource(m_ViewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix( D3DXMATRIX* ProjectionMatrix )
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, ProjectionMatrix);
	m_DeviceContext->UpdateSubresource(m_ProjectionBuffer, 0, NULL, &projection, 0, 0);
}



void Renderer::SetMaterial( MATERIAL Material )
{
	m_DeviceContext->UpdateSubresource( m_MaterialBuffer, 0, NULL, &Material, 0, 0 );
}

void Renderer::SetLight( LIGHT Light )
{
	m_DeviceContext->UpdateSubresource(m_LightBuffer, 0, NULL, &Light, 0, 0);
}

void Renderer::SetParameter(PARAMETER param)
{
	m_DeviceContext->UpdateSubresource(m_ParameterBuffer, 0, NULL, &param, 0, 0);
}

void Renderer::SetViewport(UINT width, UINT height)
{
	// ビューポート設定
	D3D11_VIEWPORT viewport;
	viewport.Width = (FLOAT)width;
	viewport.Height = (FLOAT)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &viewport);
}

void Renderer::CreateVertexShader( ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName )
{

	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreateVertexShader(buffer, fsize, NULL, VertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_Device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		VertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** PixelShader, const char* FileName )
{
	FILE* file;
	long int fsize;

	file = fopen(FileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_Device->CreatePixelShader(buffer, fsize, NULL, PixelShader);

	delete[] buffer;
}


