#include <DirectXMath.h>
#include "MyImGuiManager.h"
#include "ImGui/ImGuizmo.h"
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "SceneGui.h"
#include "inspectorGui.h"
#include "polygonCollision.h"
#include "CollisionManager.h"
#include "editorCamera.h"
#include "editorCameraObject.h"
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
	MyImGuiManager::GetInstance()->SetFocusWindow(ImGui::GetCurrentWindow());
	imgSize = ImGui::GetContentRegionAvail();
	imgPos = ImGui::GetCursorScreenPos();
	// シーン画面を画像としてレンダリング
	ImGui::Image((void*)Renderer::GetEditorShaderResourceView(), imgSize);
	// オブジェクトを選択していたならマニピュレーターを表示
	if (m_selectedObject) {
		EditorCamera* camera;
		D3DXMATRIX* viewMatrix;
		D3DXMATRIX* projectionMatrix;
		D3DXMATRIX objectMatrix;
		D3DXMatrixIdentity(&objectMatrix);

		// カメラのビュー行列とプロジェクション行列を取得
		camera = Manager::GetInstance()->GetEditor()->GetGameObject<EditorCameraObject>()->GetComponent<EditorCamera>();
		viewMatrix = camera->GetViewMatrix();
		projectionMatrix = camera->GetProjectionMatrix();
		// オブジェクトのローカル情報から行列を作成
		D3DXVECTOR3 pos = m_selectedObject->GetTransform()->m_localPosition;
		D3DXVECTOR3 rot = m_selectedObject->GetTransform()->GetLocalRotationAsDegree();
		D3DXVECTOR3 scale = m_selectedObject->GetTransform()->m_localScale;
		float worldTransArray[3];
		float worldRotArray[3];
		float worldScaleArray[3];

		// オブジェクトのワールド変換行列から
		D3DXMATRIX* worldObjMatrix = m_selectedObject->GetTransform()->GetWorldMatrix();
		ImGuizmo::DecomposeMatrixToComponents(*worldObjMatrix->m, worldTransArray, worldRotArray, worldScaleArray);

		ImGuizmo::SetDrawlist();
		ImGuiIO& io = ImGui::GetIO();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		// マニピュレーターを生成
		ImGuizmo::Manipulate(*viewMatrix->m, *projectionMatrix->m, currentGuizmoOperation, currentGuizmoMode, *worldObjMatrix->m, NULL, NULL);
		// マニピュレーターが使用されていたなら
		if (ImGuizmo::IsUsing()) {
			float transArray[3];
			float rotArray[3];
			float scaleArray[3];
			// マニピュレーターで変更されたtransform情報を取得
			ImGuizmo::DecomposeMatrixToComponents(*worldObjMatrix->m, transArray, rotArray, scaleArray);

			// transformへ送信
			D3DXVECTOR3 pos, rot, scale;
			// マニピュレーターを操作する前との差を調べて実際に動かした値を算出
			pos = D3DXVECTOR3(transArray[0] - worldTransArray[0], transArray[1] - worldTransArray[1], transArray[2] - worldTransArray[2]);
			rot = D3DXVECTOR3(rotArray[0] - worldRotArray[0], rotArray[1] - worldRotArray[1], rotArray[2] - worldRotArray[2]);
			scale = D3DXVECTOR3(scaleArray[0] - worldScaleArray[0], scaleArray[1] - worldScaleArray[1], scaleArray[2] - worldScaleArray[2]);
			// オブジェクトのローカル情報に動かした値を足す
			m_selectedObject->GetTransform()->m_localPosition += pos;
			m_selectedObject->GetTransform()->SetLocalRotationFromDegree(m_selectedObject->GetTransform()->GetLocalRotationAsDegree() + rot);
			m_selectedObject->GetTransform()->m_localScale += scale;
		}
	}

	ImVec2 mousePos;
	mousePos.x = 0;
	mousePos.y = 0;
	// シーン用ウィンドウ内で右クリックしたことを検知
	if (!ImGuizmo::IsOver()&& ImGui::GetIO().MouseDown[0] && ImGui::IsWindowFocused()) {
		mousePos = ImGui::GetMousePos();
		POINT pos = ScreenToGameScreenPoint(mousePos, imgPos, imgSize);
		auto obj = GetMousePosObject(pos);
		if (obj) {
			m_selectedObject = obj;
			InspectorGui* inspector = MyImGuiManager::GetInstance()->GetImGui<InspectorGui>();
			inspector->SetSelectedObject(obj);
		}
		
	}

	ImGui::End();

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
	mousePos.x = SCREEN_WIDTH * ratioX;
	mousePos.y = SCREEN_HEIGHT * ratioY;
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
	auto colls = CollisionManager::GetInstance()->GetPolygonCollList();

	auto camera = Manager::GetInstance()->GetEditor()->GetGameObject<EditorCameraObject>()->GetComponent<EditorCamera>();
	// ゲーム内のゲームオブジェクトの数だけループ
	for (auto coll : colls) {
		D3DXVECTOR3 world1, world2;
		// レイを判定を取るオブジェクトのローカル座標系に変換
		auto worldMatrix = coll->GetGameObject()->GetTransform()->GetWorldMatrix();
		CollisionManager::GetInstance()->ScreenToLocalPosition(worldMatrix,
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 0.0f, &world1);
		CollisionManager::GetInstance()->ScreenToLocalPosition(worldMatrix,
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 1.0f, &world2);
		// レイを作成
		D3DXVECTOR3 vec = world2 - world1;
		D3DXVec3Normalize(&vec, &vec);
		Ray ray(world1, vec);
		auto poly = coll;
		float t = -1.0f;
		// レイとポリゴンで当たり判定
		if (CollisionManager::GetInstance()->Collision_RayToPolygon(&ray, poly, &t)) {
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
