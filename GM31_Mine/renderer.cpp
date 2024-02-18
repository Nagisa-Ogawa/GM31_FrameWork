
#include "main.h"
#include "renderer.h"
#include <io.h>

//#define _CRT_SECURE_NO_WARNINGS

RENDER_TARGET			Renderer::m_renderTarget = RENDER_TARGET::ENGINE;

D3D_FEATURE_LEVEL       Renderer::m_featureLevel = D3D_FEATURE_LEVEL_11_0;

ID3D11Device*           Renderer::m_device{};
ID3D11DeviceContext*    Renderer::m_deviceContext{};
IDXGISwapChain*         Renderer::m_swapChain{};
ID3D11RenderTargetView* Renderer::m_renderTargetView{};
ID3D11DepthStencilView* Renderer::m_depthStencilView{};

ID3D11Buffer*			Renderer::m_worldBuffer{};
ID3D11Buffer*			Renderer::m_viewBuffer{};
ID3D11Buffer*			Renderer::m_projectionBuffer{};
ID3D11Buffer*			Renderer::m_materialBuffer{};
ID3D11Buffer*			Renderer::m_lightBuffer{};


ID3D11DepthStencilState* Renderer::m_depthStateEnable{};
ID3D11DepthStencilState* Renderer::m_depthStateDisable{};


ID3D11BlendState*		Renderer::m_blendState{};
ID3D11BlendState*		Renderer::m_blendStateATC{};

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


UINT Renderer::m_resizeWidth{};
UINT Renderer::m_resizeHeight{};

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
										&m_swapChain,
										&m_device,
										&m_featureLevel,
										&m_deviceContext );



	SetRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT);

	// ラスタライザステート設定
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID; 
	rasterizerDesc.CullMode = D3D11_CULL_BACK; 
	rasterizerDesc.DepthClipEnable = TRUE; 
	rasterizerDesc.MultisampleEnable = FALSE; 

	ID3D11RasterizerState *rs;
	m_device->CreateRasterizerState( &rasterizerDesc, &rs );

	m_deviceContext->RSSetState( rs );




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

	m_device->CreateBlendState( &blendDesc, &m_blendState );

	blendDesc.AlphaToCoverageEnable = TRUE;
	m_device->CreateBlendState( &blendDesc, &m_blendStateATC );

	float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_deviceContext->OMSetBlendState(m_blendState, blendFactor, 0xffffffff );





	// デプスステンシルステート設定
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	depthStencilDesc.StencilEnable = FALSE;

	m_device->CreateDepthStencilState( &depthStencilDesc, &m_depthStateEnable );//深度有効ステート

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ZERO;
	m_device->CreateDepthStencilState( &depthStencilDesc, &m_depthStateDisable );//深度無効ステート

	m_deviceContext->OMSetDepthStencilState( m_depthStateEnable, NULL );




	// サンプラーステート設定
	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MaxAnisotropy = 4;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState{};
	m_device->CreateSamplerState( &samplerDesc, &samplerState );

	m_deviceContext->PSSetSamplers( 0, 1, &samplerState );



	// 定数バッファ生成
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = sizeof(D3DXMATRIX);
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = sizeof(float);

	m_device->CreateBuffer( &bufferDesc, NULL, &m_worldBuffer );
	m_deviceContext->VSSetConstantBuffers( 0, 1, &m_worldBuffer);

	m_device->CreateBuffer( &bufferDesc, NULL, &m_viewBuffer );
	m_deviceContext->VSSetConstantBuffers( 1, 1, &m_viewBuffer );

	m_device->CreateBuffer( &bufferDesc, NULL, &m_projectionBuffer );
	m_deviceContext->VSSetConstantBuffers( 2, 1, &m_projectionBuffer );


	bufferDesc.ByteWidth = sizeof(MATERIAL);

	m_device->CreateBuffer( &bufferDesc, NULL, &m_materialBuffer );
	m_deviceContext->VSSetConstantBuffers( 3, 1, &m_materialBuffer );
	m_deviceContext->PSSetConstantBuffers( 3, 1, &m_materialBuffer );


	bufferDesc.ByteWidth = sizeof(LIGHT);

	m_device->CreateBuffer( &bufferDesc, NULL, &m_lightBuffer );
	m_deviceContext->VSSetConstantBuffers( 4, 1, &m_lightBuffer );
	m_deviceContext->PSSetConstantBuffers( 4, 1, &m_lightBuffer );





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
	m_device->CreateTexture2D(&editorVTD, NULL, &m_editorViewTexture);


	// レンダーターゲットの設定
	editorRTVD.Format = editorVTD.Format;
	editorRTVD.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	editorRTVD.Texture2D.MipSlice = 0;

	// レンダーターゲットの作成
	m_device->CreateRenderTargetView(m_editorViewTexture, &editorRTVD, &m_editorViewRenderTargetView);
	

	// シェーダーリソースビューの設定
	editorSRVD.Format = editorVTD.Format;
	editorSRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	editorSRVD.Texture2D.MostDetailedMip = 0;
	editorSRVD.Texture1D.MipLevels = 1;

	// シェーダーリソースビューの作成
	hr= m_device->CreateShaderResourceView(m_editorViewTexture, &editorSRVD, &m_editorViewShaderresourceView);


	// デプスステンシルバッファ作成
	D3D11_TEXTURE2D_DESC editorTD;
	m_editorViewTexture->GetDesc(&editorTD);
	editorTD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	editorTD.Usage = D3D11_USAGE_DEFAULT;
	editorTD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_device->CreateTexture2D(&editorTD, NULL, &m_editorViewTexture);
	m_device->CreateDepthStencilView(m_editorViewTexture, NULL, &m_editorViewDepthStencilView);



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
	m_device->CreateTexture2D(&gameVTD, NULL, &m_gameViewTexture);


	// レンダーターゲットの設定
	gameRTVD.Format = gameVTD.Format;
	gameRTVD.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	gameRTVD.Texture2D.MipSlice = 0;

	// レンダーターゲットの作成
	m_device->CreateRenderTargetView(m_gameViewTexture, &gameRTVD, &m_gameViewRenderTargetView);


	// シェーダーリソースビューの設定
	gameSRVD.Format = gameVTD.Format;
	gameSRVD.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	gameSRVD.Texture2D.MostDetailedMip = 0;
	gameSRVD.Texture1D.MipLevels = 1;

	// シェーダーリソースビューの作成
	hr = m_device->CreateShaderResourceView(m_gameViewTexture, &gameSRVD, &m_gameViewShaderresourceView);


	// デプスステンシルバッファ作成
	D3D11_TEXTURE2D_DESC gameTD;
	m_gameViewTexture->GetDesc(&gameTD);
	gameTD.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	gameTD.Usage = D3D11_USAGE_DEFAULT;
	gameTD.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_device->CreateTexture2D(&gameTD, NULL, &m_gameViewTexture);
	m_device->CreateDepthStencilView(m_gameViewTexture, NULL, &m_gameViewDepthStencilView);

}



void Renderer::Uninit()
{

	m_worldBuffer->Release();
	m_viewBuffer->Release();
	m_projectionBuffer->Release();
	m_lightBuffer->Release();
	m_materialBuffer->Release();


	m_deviceContext->ClearState();
	m_renderTargetView->Release();
	m_swapChain->Release();
	m_deviceContext->Release();
	m_device->Release();

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
	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_renderTargetView, clearColor );
	m_deviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::EditorViewBegin()
{
	m_renderTarget = RENDER_TARGET::EDITOR;
	// ビューポートを変更
	SetViewport(GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT);
	m_deviceContext->OMSetRenderTargets(1, &m_editorViewRenderTargetView, m_editorViewDepthStencilView);
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_editorViewRenderTargetView, clearColor);
	m_deviceContext->ClearDepthStencilView(m_editorViewDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Renderer::GameViewBegin()
{
	m_renderTarget = RENDER_TARGET::GAME;
	// ビューポートを変更
	SetViewport(GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT);
	m_deviceContext->OMSetRenderTargets(1, &m_gameViewRenderTargetView, m_gameViewDepthStencilView);
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_deviceContext->ClearRenderTargetView(m_gameViewRenderTargetView, clearColor);
	m_deviceContext->ClearDepthStencilView(m_gameViewDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}




void Renderer::End()
{
	m_swapChain->Present( 1, 0 );
}


void Renderer::SetRenderTarget(UINT width, UINT height)
{
	ID3D11Texture2D* renderTarget{};
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&renderTarget);
	m_device->CreateRenderTargetView(renderTarget, NULL, &m_renderTargetView);
	renderTarget->Release();

	// デプスステンシルバッファ作成
	ID3D11Texture2D* depthStencile{};
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = width;
	textureDesc.Height = width;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D16_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	m_device->CreateTexture2D(&textureDesc, NULL, &depthStencile);

	// デプスステンシルビュー作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	depthStencilViewDesc.Format = textureDesc.Format;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Flags = 0;
	m_device->CreateDepthStencilView(depthStencile, &depthStencilViewDesc, &m_depthStencilView);
	depthStencile->Release();

	m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);

	SetViewport(width, height);
}

void Renderer::CleanRenderTarget()
{
	if (m_renderTargetView) { m_renderTargetView->Release(); m_renderTargetView = nullptr; }
	if (m_depthStencilView) { m_depthStencilView->Release(); m_depthStencilView = nullptr; }
}


void Renderer::SetDepthEnable( bool enable )
{
	if( enable )
		m_deviceContext->OMSetDepthStencilState( m_depthStateEnable, NULL );
	else
		m_deviceContext->OMSetDepthStencilState( m_depthStateDisable, NULL );

}



void Renderer::SetATCEnable( bool enable )
{
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	if (enable)
		m_deviceContext->OMSetBlendState(m_blendStateATC, blendFactor, 0xffffffff);
	else
		m_deviceContext->OMSetBlendState(m_blendState, blendFactor, 0xffffffff);

}


void Renderer::SetWorldViewProjection2D()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranspose(&world, &world);

	m_deviceContext->UpdateSubresource(m_worldBuffer, 0, NULL, &world, 0, 0);

	D3DXMATRIX view;
	D3DXMatrixIdentity(&view);
	D3DXMatrixTranspose(&view, &view);
	m_deviceContext->UpdateSubresource(m_viewBuffer, 0, NULL, &view, 0, 0);

	D3DXMATRIX projection;
	D3DXMatrixOrthoOffCenterLH(&projection, 0.0f, GAMESCREEN_WIDTH, GAMESCREEN_HEIGHT, 0.0f, 0.0f, 1.0f);
	D3DXMatrixTranspose(&projection, &projection);
	m_deviceContext->UpdateSubresource( m_projectionBuffer, 0, NULL, &projection, 0, 0 );

}


void Renderer::SetWorldMatrix( D3DXMATRIX* worldMatrix )
{
	D3DXMATRIX world;
	D3DXMatrixTranspose(&world, worldMatrix);
	m_deviceContext->UpdateSubresource(m_worldBuffer, 0, NULL, &world, 0, 0);
}

void Renderer::SetViewMatrix( D3DXMATRIX* viewMatrix )
{
	D3DXMATRIX view;
	D3DXMatrixTranspose(&view, viewMatrix);
	m_deviceContext->UpdateSubresource(m_viewBuffer, 0, NULL, &view, 0, 0);
}

void Renderer::SetProjectionMatrix( D3DXMATRIX* projectionMatrix )
{
	D3DXMATRIX projection;
	D3DXMatrixTranspose(&projection, projectionMatrix);
	m_deviceContext->UpdateSubresource(m_projectionBuffer, 0, NULL, &projection, 0, 0);
}



void Renderer::SetMaterial( MATERIAL material )
{
	m_deviceContext->UpdateSubresource( m_materialBuffer, 0, NULL, &material, 0, 0 );
}

void Renderer::SetLight( LIGHT light )
{
	m_deviceContext->UpdateSubresource(m_lightBuffer, 0, NULL, &light, 0, 0);
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
	m_deviceContext->RSSetViewports(1, &viewport);
}

void Renderer::CreateVertexShader( ID3D11VertexShader** vertexShader, ID3D11InputLayout** vertexLayout, const char* fileName )
{

	FILE* file;
	long int fsize;

	file = fopen(fileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_device->CreateVertexShader(buffer, fsize, NULL, vertexShader);


	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 4 * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 4 * 6, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 4 * 10, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	m_device->CreateInputLayout(layout,
		numElements,
		buffer,
		fsize,
		vertexLayout);

	delete[] buffer;
}



void Renderer::CreatePixelShader( ID3D11PixelShader** pixelShader, const char* fileName )
{
	FILE* file;
	long int fsize;

	file = fopen(fileName, "rb");
	assert(file);

	fsize = _filelength(_fileno(file));
	unsigned char* buffer = new unsigned char[fsize];
	fread(buffer, fsize, 1, file);
	fclose(file);

	m_device->CreatePixelShader(buffer, fsize, NULL, pixelShader);

	delete[] buffer;
}


