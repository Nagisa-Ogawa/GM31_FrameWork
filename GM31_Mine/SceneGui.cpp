#include <DirectXMath.h>
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
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
	static ImGuizmo::OPERATION currentGuizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE currentGuizmoMode(ImGuizmo::LOCAL);


	// オブジェクトを選択していたならマニピュレーターを表示
	if (m_selectedObject) {
		if (ImGui::IsKeyPressed(ImGuiKey_T))
			currentGuizmoOperation = ImGuizmo::TRANSLATE;
		if (ImGui::IsKeyPressed(ImGuiKey_R))
			currentGuizmoOperation = ImGuizmo::ROTATE;
		if (ImGui::IsKeyPressed(ImGuiKey_Y)) // r Key
			currentGuizmoOperation = ImGuizmo::SCALE;

	}

	ImVec2 imgSize;
	ImVec2 imgPos;

	ImGui::Begin("Scene");
	imgSize = ImGui::GetContentRegionAvail();
	imgPos = ImGui::GetCursorScreenPos();
	// シーン画面を画像としてレンダリング
	ImGui::Image((void*)Renderer::GetEditorShaderResourceView(), imgSize);

	// オブジェクトを選択していたならマニピュレーターを表示
	if (m_selectedObject) {
		Camera* camera;
		D3DXMATRIX* viewMatrix;
		D3DXMATRIX* projectionMatrix;
		D3DXMATRIX objectMatrix;
		D3DXMatrixIdentity(&objectMatrix);

		// カメラのビュー行列とプロジェクション行列を取得
		camera = Manager::GetInstance()->GetEditor()->GetGameObject<CameraObject>()->GetComponent<Camera>();
		viewMatrix = camera->GetViewMatrix();
		projectionMatrix = camera->GetProjectionMatrix();
		// オブジェクトのtransformの情報から行列を作成
		D3DXVECTOR3 pos = m_selectedObject->GetTransform()->m_position;
		D3DXVECTOR3 rot = m_selectedObject->GetTransform()->GetRotationAsDegree();
		D3DXVECTOR3 scale = m_selectedObject->GetTransform()->m_scale;
		float transArray[3] = { pos.x,pos.y,pos.z };
		float	rotArray[3] = { rot.x,rot.y,rot.z };
		float	scaleArray[3] = { scale.x,scale.y,scale.z };
		ImGuizmo::RecomposeMatrixFromComponents(transArray, rotArray, scaleArray, &objectMatrix.m[0][0]);

		ImGuizmo::SetDrawlist();
		ImGuiIO& io = ImGui::GetIO();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// マニピュレーターを生成
		ImGuizmo::Manipulate(*viewMatrix->m, *projectionMatrix->m, currentGuizmoOperation, currentGuizmoMode, &objectMatrix.m[0][0], NULL, NULL);
		// マニピュレーターが使用されていたなら
		if (ImGuizmo::IsUsing()) {
			// マニピュレーターで変更されたtransform情報を取得
			ImGuizmo::DecomposeMatrixToComponents(&objectMatrix.m[0][0], transArray, rotArray, scaleArray);

			// transformへ送信
			m_selectedObject->GetTransform()->m_position = D3DXVECTOR3(transArray[0], transArray[1], transArray[2]);
			m_selectedObject->GetTransform()->SetRotationFromDegree(D3DXVECTOR3(rotArray[0], rotArray[1], rotArray[2]));
			m_selectedObject->GetTransform()->m_scale = D3DXVECTOR3(scaleArray[0], scaleArray[1], scaleArray[2]);
		}
	}

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

	auto camera = Manager::GetInstance()->GetEditor()->GetGameObject<CameraObject>()->GetComponent<Camera>();
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
