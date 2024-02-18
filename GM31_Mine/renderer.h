#pragma once


enum RENDER_TARGET
{
	ENGINE,
	EDITOR,
	GAME,
};


struct VERTEX_3D
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR4 Diffuse;
    D3DXVECTOR2 TexCoord;
};



struct MATERIAL
{
	D3DXCOLOR	Ambient;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Specular;
	D3DXCOLOR	Emission;
	float		Shininess;
	BOOL		TextureEnable;
	float		Dummy[2];
};



struct LIGHT
{
	BOOL		Enable;
	BOOL		Dummy[3];
	D3DXVECTOR4	Direction;
	D3DXCOLOR	Diffuse;
	D3DXCOLOR	Ambient;
};


//-----------------------------------------------------
// 描画クラス
//------------------------------------------------------
class Renderer
{
private:

	static RENDER_TARGET m_renderTarget;

	static D3D_FEATURE_LEVEL       m_featureLevel;

	static ID3D11Device*           m_device;
	static ID3D11DeviceContext*    m_deviceContext;
	static IDXGISwapChain*         m_swapChain;
	static ID3D11RenderTargetView* m_renderTargetView;
	static ID3D11DepthStencilView* m_depthStencilView;

	static ID3D11Buffer*			m_worldBuffer;
	static ID3D11Buffer*			m_viewBuffer;
	static ID3D11Buffer*			m_projectionBuffer;
	static ID3D11Buffer*			m_materialBuffer;
	static ID3D11Buffer*			m_lightBuffer;


	static ID3D11DepthStencilState* m_depthStateEnable;
	static ID3D11DepthStencilState* m_depthStateDisable;

	static ID3D11BlendState*		m_blendState;
	static ID3D11BlendState*		m_blendStateATC;

	// エディタビュー用テクスチャ
	static ID3D11Texture2D*			m_editorViewTexture;
	static ID3D11RenderTargetView*  m_editorViewRenderTargetView;
	static ID3D11DepthStencilView*	m_editorViewDepthStencilView;
	static ID3D11ShaderResourceView* m_editorViewShaderresourceView;

	// ゲームビュー用テクスチャ
	static ID3D11Texture2D* m_gameViewTexture;
	static ID3D11RenderTargetView* m_gameViewRenderTargetView;
	static ID3D11DepthStencilView* m_gameViewDepthStencilView;
	static ID3D11ShaderResourceView* m_gameViewShaderresourceView;


	// ウィンドウのサイズが変わった際のサイズ
	static UINT		m_resizeWidth;
	static UINT		m_resizeHeight;
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void EditorViewBegin();
	static void GameViewBegin();
	static void End();

	static void SetDepthEnable(bool enable);
	static void SetATCEnable(bool enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* worldMatrix);
	static void SetViewMatrix(D3DXMATRIX* viewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* projectionMatrix);
	static void SetMaterial(MATERIAL material);
	static void SetLight(LIGHT light);
	static void SetResizeWidth(UINT width) { m_resizeWidth = width; }
	static void SetResizeHeight(UINT height) { m_resizeHeight = height; }
	static void SetViewport(UINT width, UINT height);

	// Get系関数
	static ID3D11Device* GetDevice( void ){ return m_device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_deviceContext; }
	static ID3D11RenderTargetView* GetRenderTargetView() { return m_renderTargetView; }
	static RENDER_TARGET GetRenderTarget() { return m_renderTarget; }
	// エディタビュー用Get関数
	static ID3D11RenderTargetView* GetEditorViewRenderTargetView() { return m_editorViewRenderTargetView; }
	static ID3D11ShaderResourceView* GetEditorShaderResourceView() { return m_editorViewShaderresourceView; }
	// ゲームビュー用Get関数
	static ID3D11RenderTargetView* GetGameViewRenderTargetView() { return m_gameViewRenderTargetView; }
	static ID3D11ShaderResourceView* GetGameShaderResourceView() { return m_gameViewShaderresourceView; }
	static IDXGISwapChain* GetSwapChain() { return m_swapChain; }
	static UINT GetResizeWidth() { return m_resizeWidth; }
	static UINT GetResizeHeight() { return m_resizeHeight; }

	/// <summary>
	/// レンダーターゲットを作成する関数
	/// </summary>
	static void SetRenderTarget(UINT width, UINT height);

	/// <summary>
	/// レンダーターゲットをクリアする関数
	/// </summary>
	static void CleanRenderTarget();

	static void CreateVertexShader(ID3D11VertexShader** vertexShader, ID3D11InputLayout** vertexLayout, const char* fileName);
	static void CreatePixelShader(ID3D11PixelShader** pixelShader, const char* fileName);


};
