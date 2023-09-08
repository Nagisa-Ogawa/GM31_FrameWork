#include "main.h"
#include "scene.h"
#include "manager.h"
#include "game.h"
#include "MyImGuiManager.h"
#include "renderer.h"
#include "player.h"

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
	static float f = 0.0f;
	static int counter = 0;

	ImGui::Begin("GameInfo");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", pio->Framerate, 1000.0f / pio->Framerate);
	ImGui::End();
	auto nowScene = Manager::GetInstance()->GetScene();
	if (typeid(*nowScene) == typeid(Game)) {
		ImGui::Begin("PlayerInfo");

		auto player = Manager::GetInstance()->GetScene()->GetGameObject<Player>();
		if (ImGui::TreeNode("Position")) {
			ImGui::Text("x:%.3f y:%.3f z:%.3f", 
				player->GetTransform()->m_Position.x, player->GetTransform()->m_Position.y, player->GetTransform()->m_Position.z);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Rotation")) {
			ImGui::Text("x:%.3f y:%.3f z:%.3f",
				player->GetTransform()->m_Rotation.x, player->GetTransform()->m_Rotation.y, player->GetTransform()->m_Rotation.z);
			ImGui::TreePop();
		}
		ImGui::Checkbox("HitEnemy", player->GetIsHitEnemy());
		ImGui::Checkbox("HitWall", player->GetIsHitWall());
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
