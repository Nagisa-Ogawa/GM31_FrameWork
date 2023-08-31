#include "main.h"
#include "scene.h"
#include "MyImGuiManager.h"
#include "renderer.h"

MyImGuiManager* MyImGuiManager::m_Instance = NULL;

// Our state
ImGuiIO* pio = nullptr;

MyImGuiManager::MyImGuiManager()
{
    // なにもしない
}

MyImGuiManager::MyImGuiManager(const MyImGuiManager& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
}

MyImGuiManager& MyImGuiManager::operator=(const MyImGuiManager& manager)
{
	// インスタンスをコピー
	m_Instance = manager.m_Instance;
	return *this;
}


MyImGuiManager::~MyImGuiManager()
{
	// インスタンスを解放
	delete m_Instance;
}

void MyImGuiManager::Init(HWND hwnd)
{
	// ImGuiの登録
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();(void)io;
	pio = &io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(Renderer::GetDevice(), Renderer::GetDeviceContext());

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
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Debug Window");                          // Create a window called "Hello, world!" and append into it.

		ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", pio->Framerate, 1000.0f / pio->Framerate);
		ImGui::End();
	}
}

void MyImGuiManager::Draw()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

MyImGuiManager* MyImGuiManager::GetInstance()
{
	// 初めて使うときにインスタンスを生成
	// それ以降は生成したインスタンスを渡す
	if (m_Instance == NULL)
	{
		m_Instance = new MyImGuiManager();
	}
	return m_Instance;

}
