
#include "main.h"
#include "renderer.h"
#include "input.h"
#include "MyImGuiManager.h"
#include "MyImGui.h"
#include "ImGui/ImGuizmo.h"
#include "mainMenuBarGui.h"
#include "SceneGui.h"
#include "consoleGui.h"
#include "engineDebugGui.h"
#include "hierarchyGui.h"
#include "inspectorGui.h"
#include "projectGui.h"
#include "gameGui.h"

MyImGuiManager* MyImGuiManager::m_instance = NULL;



MyImGuiManager::MyImGuiManager()
{
    // なにもしない
}

MyImGuiManager::MyImGuiManager(const MyImGuiManager& manager)
{
	// インスタンスをコピー
	m_instance = manager.m_instance;
}

MyImGuiManager& MyImGuiManager::operator=(const MyImGuiManager& manager)
{
	// インスタンスをコピー
	m_instance = manager.m_instance;
	return *this;
}


MyImGuiManager::~MyImGuiManager()
{
	// インスタンスを解放
	delete m_instance;
}

void MyImGuiManager::Init(HWND hwnd)
{
	// ImGuiの登録
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();(void)io;
	// キーボードとコントローラーの入力を有効化
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
	// ドッキングウィンドウを有効化
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; 
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	// io.ConfigWindowsMoveFromTitleBarOnly = TRUE;

	// カラースタイルを選択
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	
	// imguiの初期化処理を呼び出し
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

	// Guiを追加
	AddImGui<MainMenuBarGui>();
	AddImGui<EngineDebugGui>();
	AddImGui<SceneGui>();
	AddImGui<GameGui>();
	AddImGui<InspectorGui>();
	AddImGui<HierarchyGui>();
	AddImGui<ProjectGui>();
	AddImGui<ConsoleGui>();
}

void MyImGuiManager::Uninit()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void MyImGuiManager::Update()
{

	if (Renderer::GetResizeWidth() != 0 && Renderer::GetResizeHeight() != 0)
	{
		// レンダーターゲットをクリア
		Renderer::CleanRenderTarget();
		// スワップチェーンのバッファサイズを変更されたウィンドウのサイズに変更
		Renderer::GetSwapChain()->ResizeBuffers(0, Renderer::GetResizeWidth(), Renderer::GetResizeHeight(), DXGI_FORMAT_UNKNOWN, 0);
		// レンダーターゲットを再作成
		Renderer::SetRenderTarget(Renderer::GetResizeWidth(), Renderer::GetResizeHeight());
		Renderer::SetResizeWidth(0); Renderer::SetResizeHeight(0);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();


	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());


	for (MyImGui* myImGui : m_myImGuiList)
	{
		myImGui->Update();
	}
}

void MyImGuiManager::Draw()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}



/// <summary>
/// インスタンスがなければ生成し、あればそれを返す関数
/// </summary>
/// <returns>インスタンス</returns>
MyImGuiManager* MyImGuiManager::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
	if (m_instance == NULL)
	{
		m_instance = new MyImGuiManager();
	}
	return m_instance;

}


/// <summary>
/// そのウィンドウにフォーカスがあっているならセットする関数
/// </summary>
/// <param name="window"></param>
void MyImGuiManager::SetFocusWindow(ImGuiWindow* window)
{
	if (ImGui::IsWindowFocused()) {
		// すでにフォーカスされているかチェック
		if (m_focusWindow==nullptr|| window->ID != m_focusWindow->ID) {
			m_focusWindow = window;
		}
	}
}
