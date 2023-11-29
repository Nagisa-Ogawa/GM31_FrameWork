#include "ImGui/imgui.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "MyImGuiManager.h"
#include "SceneGui.h"
#include "inspectorGui.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "boxCollisionFrame.h"
#include "camera.h"
#include "cameraObject.h"
#include "Ray.h"


void SceneGui::Init()
{
}

void SceneGui::Update()
{
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
		POINT pos = ScreenToGameScreenPoint(mousePos, imgPos, imgSize);
		auto obj = GetMousePosObject(pos);
		if (obj) {
			m_forcusObject = obj;
			InspectorGui* inspector = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
			inspector->SetForcusObject(obj);
		}
			
	}

}


/// <summary>
/// 通常の画面で取得した座標をゲーム画面をレンダリングしている画面での座標に変換
/// </summary>
/// <param name="pos">ゲーム画像の左上を(0,0)としたときの座標</param>
/// <param name="imgSize">画像のサイズ</param>
/// <returns>変換後の座標</returns>
POINT SceneGui::ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize)
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
GameObject* SceneGui::GetMousePosObject(POINT mousePos)
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
				else if (t < minT) {
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
