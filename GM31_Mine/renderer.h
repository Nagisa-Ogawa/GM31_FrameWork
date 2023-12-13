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



class Renderer
{
private:

	static RENDER_TARGET m_renderTarget;

	static D3D_FEATURE_LEVEL       m_FeatureLevel;

	static ID3D11Device*           m_Device;
	static ID3D11DeviceContext*    m_DeviceContext;
	static IDXGISwapChain*         m_SwapChain;
	static ID3D11RenderTargetView* m_RenderTargetView;
	static ID3D11DepthStencilView* m_DepthStencilView;

	static ID3D11Buffer*			m_WorldBuffer;
	static ID3D11Buffer*			m_ViewBuffer;
	static ID3D11Buffer*			m_ProjectionBuffer;
	static ID3D11Buffer*			m_MaterialBuffer;
	static ID3D11Buffer*			m_LightBuffer;


	static ID3D11DepthStencilState* m_DepthStateEnable;
	static ID3D11DepthStencilState* m_DepthStateDisable;

	static ID3D11BlendState*		m_BlendState;
	static ID3D11BlendState*		m_BlendStateATC;

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
	static UINT		m_ResizeWidth;
	static UINT		m_ResizeHeight;
public:
	static void Init();
	static void Uninit();
	static void Begin();
	static void EditorViewBegin();
	static void GameViewBegin();
	static void End();

	static void SetDepthEnable(bool Enable);
	static void SetATCEnable(bool Enable);
	static void SetWorldViewProjection2D();
	static void SetWorldMatrix(D3DXMATRIX* WorldMatrix);
	static void SetViewMatrix(D3DXMATRIX* ViewMatrix);
	static void SetProjectionMatrix(D3DXMATRIX* ProjectionMatrix);
	static void SetMaterial(MATERIAL Material);
	static void SetLight(LIGHT Light);
	static void SetResizeWidth(UINT width) { m_ResizeWidth = width; }
	static void SetResizeHeight(UINT height) { m_ResizeHeight = height; }
	static void SetViewport(UINT width, UINT height);

	// Get系関数
	static ID3D11Device* GetDevice( void ){ return m_Device; }
	static ID3D11DeviceContext* GetDeviceContext( void ){ return m_DeviceContext; }
	static ID3D11RenderTargetView* GetRenderTargetView() { return m_RenderTargetView; }
	static RENDER_TARGET GetRenderTarget() { return m_renderTarget; }
	// エディタビュー用Get関数
	static ID3D11RenderTargetView* GetEditorViewRenderTargetView() { return m_editorViewRenderTargetView; }
	static ID3D11ShaderResourceView* GetEditorShaderResourceView() { return m_editorViewShaderresourceView; }
	// ゲームビュー用Get関数
	static ID3D11RenderTargetView* GetGameViewRenderTargetView() { return m_gameViewRenderTargetView; }
	static ID3D11ShaderResourceView* GetGameShaderResourceView() { return m_gameViewShaderresourceView; }
	static IDXGISwapChain* GetSwapChain() { return m_SwapChain; }
	static UINT GetResizeWidth() { return m_ResizeWidth; }
	static UINT GetResizeHeight() { return m_ResizeHeight; }

	/// <summary>
	/// レンダーターゲットを作成する関数
	/// </summary>
	static void SetRenderTarget(UINT width, UINT height);

	/// <summary>
	/// レンダーターゲットをクリアする関数
	/// </summary>
	static void CleanRenderTarget();



	static void CreateVertexShader(ID3D11VertexShader** VertexShader, ID3D11InputLayout** VertexLayout, const char* FileName);
	static void CreatePixelShader(ID3D11PixelShader** PixelShader, const char* FileName);


};
