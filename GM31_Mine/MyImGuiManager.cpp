#include "main.h"
#include "scene.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "MyImGuiManager.h"
#include "player.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "boxCollisionFrame.h"
#include "camera.h"
#include "cameraObject.h"
#include "Ray.h"

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
	auto nowScene = Manager::GetInstance()->GetScene();

	ImGui::Begin("GameInfo");                          // Create a window called "Hello, world!" and append into it.

	ImGui::Text(" %.1f FPS (%.3f ms/frame)  ", pio->Framerate, 1000.0f / pio->Framerate);
	ImGui::Text(" ObjectCount : %d", Manager::GetInstance()->GetScene()->GetGameObjectCount());
	ImGui::Text(" ActiveObjectCount : %d", Manager::GetInstance()->GetScene()->GetActiveGameObjectCount());
	if (typeid(*nowScene) == typeid(Game)) {
		if (ImGui::Checkbox(" Show Collision", &m_IsShowColl)) {
			auto colls = Manager::GetInstance()->GetScene()->GetGameObjects<BoxCollisionFrame>();
			for (auto coll : colls) {
				coll->SetActive(m_IsShowColl);
			}
		}
	}
	ImGui::End();

	if (typeid(*nowScene) == typeid(Game)) {
		if (Input::GetKeyPress(VK_LBUTTON)) {
			auto obj = GetMousePosObject();
			if (obj)
				m_InfoObj = obj;
		}
		if (m_InfoObj) {
			ImGui::Begin("ObjectInfo");
			ImGui::Text("ObjectType : %s", typeid(*m_InfoObj).name());
			if (ImGui::TreeNode("Position")) {
				ImGui::Text("x:%.3f y:%.3f z:%.3f",
					m_InfoObj->GetTransform()->m_Position.x, m_InfoObj->GetTransform()->m_Position.y, m_InfoObj->GetTransform()->m_Position.z);
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Rotation")) {
				ImGui::Text("x:%.3f y:%.3f z:%.3f",
					m_InfoObj->GetTransform()->m_Rotation.x, m_InfoObj->GetTransform()->m_Rotation.y, m_InfoObj->GetTransform()->m_Rotation.z);
				ImGui::TreePop();
			}
			ImGui::End();
		}
	}
}

void MyImGuiManager::Draw()
{
	// Rendering
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}


// マウス座標にオブジェクトがあるかを調べそのオブジェクトを返す関数
GameObject* MyImGuiManager::GetMousePosObject()
{
	GameObject* obj = nullptr;
	float minT = -10.0f;
	auto colls = CollisionManager::GetInstance()->GetBoxCollList();
	auto mousePos = Input::GetClientMousePos();
	auto camera = Manager::GetInstance()->GetScene()->GetGameObject<CameraObject>()->GetComponent<Camera>();
	for (auto coll : colls) {
		D3DXVECTOR3 world1, world2;
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
