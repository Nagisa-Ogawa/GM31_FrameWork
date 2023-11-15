
#include "main.h"
#include "scene.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "MyImGuiManager.h"
#include "MyImGui.h"
#include "player.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "boxCollisionFrame.h"
#include "camera.h"
#include "cameraObject.h"
#include "Ray.h"

MyImGuiManager* MyImGuiManager::m_instance = NULL;

// Our state
ImGuiIO* pio = nullptr;

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
	pio = &io;
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
	auto nowScene = Manager::GetInstance()->GetScene();

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

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	
	ImVec2 imgSize;
	ImVec2 imgPos;
	ImGui::Begin("Scene");
		imgSize = ImGui::GetContentRegionAvail();
		imgPos = ImGui::GetCursorScreenPos();
		ImGui::Image((void*)Renderer::GetGameShaderResourceView(), imgSize);
		ImGui::End();

	ImVec2 mousePos;
	mousePos.x = 0;
	mousePos.y = 0;
	// シーン用ウィンドウ内で右クリックしたことを検知
	if (ImGui::GetIO().MouseDown[0]) {
		mousePos = ImGui::GetMousePos();
		POINT pos = ScreenToGameScreenPoint(mousePos, imgPos,imgSize);
		auto obj = GetMousePosObject(pos);
		if (obj)
			m_infoObj = obj;
	}

	ImGui::Begin("Game");
		ImVec2 imageSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)Renderer::GetGameShaderResourceView(), imageSize);
		ImGui::End();

	ImGui::Begin("EngineDebug");
		ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", pio->Framerate, 1000.0f / pio->Framerate);
		ImGui::Text(" ObjectCount : %d", Manager::GetInstance()->GetScene()->GetGameObjectCount());
		ImGui::Text(" ActiveObjectCount : %d", Manager::GetInstance()->GetScene()->GetActiveGameObjectCount());
		if (ImGui::Checkbox(" Show Collision", &m_isShowColl)) {
			auto colls = Manager::GetInstance()->GetScene()->GetGameObjects<BoxCollisionFrame>();
			for (auto coll : colls) {
				coll->SetActive(m_isShowColl);
			}
		}
		ImGui::End();

	ImGui::Begin("Inspecter");
		if (m_infoObj) {
			ImGui::Text("ObjectType : %s", typeid(*m_infoObj).name());
			if (ImGui::TreeNode("Position")) {
				ImGui::Text("x:%.3f y:%.3f z:%.3f",
					m_infoObj->GetTransform()->m_position.x, m_infoObj->GetTransform()->m_position.y, m_infoObj->GetTransform()->m_position.z);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rotation")) {
				ImGui::Text("x:%.3f y:%.3f z:%.3f",
					m_infoObj->GetTransform()->m_rotation.x, m_infoObj->GetTransform()->m_rotation.y, m_infoObj->GetTransform()->m_rotation.z);
				ImGui::TreePop();
			}
		}
		ImGui::End();

	ImGui::Begin("Project");
		ImGui::End();

	ImGui::Begin("Hierarchy");
		ImGui::End();

	ImGui::Begin("Console");
		
		ImGui::End();
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
/// 通常の画面で取得した座標をゲーム画面をレンダリングしている画面での座標に変換
/// </summary>
/// <param name="pos">ゲーム画像の左上を(0,0)としたときの座標</param>
/// <param name="imgSize">画像のサイズ</param>
/// <returns>変換後の座標</returns>
POINT MyImGuiManager::ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize)
{
	POINT mousePos;
	// ウィンドウの左上を(0,0)とした座標に変換
	POINT cPos, cImgPos;
	cPos.x = (int)pos.x; cPos.y = (int)pos.y;
	cImgPos.x = (int)imgPos.x; cImgPos.y = (int)imgPos.y;
	ScreenToClient(GetWindow(), &cPos);
	ScreenToClient(GetWindow(), &cImgPos);

	cPos.x -= cImgPos.x;
	cPos.y -= cImgPos.y;
	float ratioX = cPos.x / imgSize.x;
	float ratioY = cPos.y / imgSize.y;
	mousePos.x = GAMESCREEN_WIDTH * ratioX;
	mousePos.y = GAMESCREEN_HEIGHT * ratioY;
	return mousePos;
}


/// <summary>
/// マウス座標にオブジェクトがあるかを調べそのオブジェクトを返す関数
/// </summary>
/// <param name="mousePos">マウス座標</param>
/// <returns>その座標にあったオブジェクト</returns>
GameObject* MyImGuiManager::GetMousePosObject(POINT mousePos)
{
	GameObject* obj = nullptr;
	float minT = -10.0f;
	auto colls = CollisionManager::GetInstance()->GetBoxCollList();

	auto camera = Manager::GetInstance()->GetScene()->GetGameObject<CameraObject>()->GetComponent<Camera>();
	// ゲーム内のゲームオブジェクトの数だけループ
	for (auto coll : colls) {
		D3DXVECTOR3 world1, world2;
		// レイを判定を取るオブジェクトのローカル座標系に変換
		auto worldMatrix = coll->GetWorldMatrix();
		CollisionManager::GetInstance()->ScreenToLocalPosition(&worldMatrix,
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 0.0f, &world1);
		CollisionManager::GetInstance()->ScreenToLocalPosition(&worldMatrix,
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 1.0f, &world2);
		// レイを作成
		D3DXVECTOR3 vec = world2 - world1;
		D3DXVec3Normalize(&vec, &vec);
		Ray ray(world1, vec);
		auto box = coll;
		float t = -1.0f;
		// レイと球体で当たり判定
		if (CollisionManager::GetInstance()->Collision_RayToBox(&ray, box, &t, NULL)) {
			if (t >= 0.0f) {
				if (minT < 0.0f) {
					obj = coll->GetGameObject();
					minT = t;
				}
				else if(t<minT){
					obj = coll->GetGameObject();
					minT = t;
				}
			}
		}
	}
	if (obj) {
		return obj;
	}
	return nullptr;
}

template<typename T>
T* MyImGuiManager::AddImGui()
{
	MyImGui* myImGui = new T();
	m_myImGuiList.push_back(myImGui);
	myImGui->Init();

	return (T*)myImGui;
}

template<typename T>
T* MyImGuiManager::GetImGui()
{
	for (MyImGui* myImGui : m_myImGuiList)
	{
		if (typeid(*myImGui) == typeid(T))// 型を調べる(RTTI動的型情報)
		{
			return (T*)myImGui;
		}
	}
	return nullptr;
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
