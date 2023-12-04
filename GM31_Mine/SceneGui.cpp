#include "ImGui/imgui.h"
#include "ImGui/ImGuizmo.h"
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
			m_selectedObject = obj;
			InspectorGui* inspector = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
			inspector->SetForcusObject(obj);
		}
			
	}
	// オブジェクトを選択していたならマニピュレーターを表示
	if (m_selectedObject) {

		auto camera = Manager::GetInstance()->GetScene()->GetGameObject<CameraObject>()->GetComponent<Camera>();
		auto viewMatrix = camera->GetViewMatrix();
		auto projectionMatrix = camera->GetProjectionMatrix();
		auto objectMatrix = m_selectedObject->GetTransform()->GetWorldMatrix();

		float objectMatrixAsFloatArray[16] = {
			objectMatrix->_11,objectMatrix->_12,objectMatrix->_13,objectMatrix->_14,
			objectMatrix->_21,objectMatrix->_22,objectMatrix->_23,objectMatrix->_24,
			objectMatrix->_31,objectMatrix->_32,objectMatrix->_33,objectMatrix->_34,
			objectMatrix->_41,objectMatrix->_42,objectMatrix->_43,objectMatrix->_44,
		};
		float viewMatrixAsFloatArray[16] = {
			viewMatrix->_11,viewMatrix->_12,viewMatrix->_13,viewMatrix->_14,
			viewMatrix->_21,viewMatrix->_22,viewMatrix->_23,viewMatrix->_24,
			viewMatrix->_31,viewMatrix->_32,viewMatrix->_33,viewMatrix->_34,
			viewMatrix->_41,viewMatrix->_42,viewMatrix->_43,viewMatrix->_44,
		};
		float projectionMatrixAsFloatArray[16] = {
			projectionMatrix->_11,projectionMatrix->_12,projectionMatrix->_13,projectionMatrix->_14,
			projectionMatrix->_21,projectionMatrix->_22,projectionMatrix->_23,projectionMatrix->_24,
			projectionMatrix->_31,projectionMatrix->_32,projectionMatrix->_33,projectionMatrix->_34,
			projectionMatrix->_41,projectionMatrix->_42,projectionMatrix->_43,projectionMatrix->_44,
		};



		static ImGuizmo::OPERATION currentGuizmoOperation(ImGuizmo::TRANSLATE);
		static ImGuizmo::MODE currentGuizmoMode(ImGuizmo::LOCAL);

		ImGuiIO& io = ImGui::GetIO();
		// ImGuizmo::SetRect(imgPos.x, imgPos.y, imgPos.x+imgSize.x, imgPos.y + imgSize.y);
		ImGuizmo::SetRect(0.0f, 0.0f, io.DisplaySize.x, io.DisplaySize.y);
		ImGuizmo::Manipulate(viewMatrixAsFloatArray, projectionMatrixAsFloatArray, currentGuizmoOperation, currentGuizmoMode, objectMatrixAsFloatArray, NULL, NULL);

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
