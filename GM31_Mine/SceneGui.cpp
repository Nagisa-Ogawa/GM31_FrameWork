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
	Camera* camera;
	D3DXMATRIX* viewMatrix;
	D3DXMATRIX* projectionMatrix;
	D3DXMATRIX* objectMatrix;
	static ImGuizmo::OPERATION currentGuizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE currentGuizmoMode(ImGuizmo::LOCAL);
	static ImGuiWindowFlags gizmoWindowFlags = 0;


	// オブジェクトを選択していたならマニピュレーターを表示
	if (m_selectedObject) {
		camera = Manager::GetInstance()->GetScene()->GetGameObject<CameraObject>()->GetComponent<Camera>();
		viewMatrix = camera->GetViewMatrix();
		projectionMatrix = camera->GetProjectionMatrix();
		objectMatrix = m_selectedObject->GetTransform()->GetWorldMatrix();

		ImGui::Text(ImGuizmo::IsOver(ImGuizmo::TRANSLATE) ? "Over translate gizmo" : "not Over");

		D3DXVECTOR3 pos = m_selectedObject->GetTransform()->m_position;
		D3DXVECTOR3 rot = m_selectedObject->GetTransform()->m_rotation;
		D3DXVECTOR3 scale = m_selectedObject->GetTransform()->m_scale;
		float matrixTranslation[3] = { pos.x,pos.y,pos.z };
		float	matrixRotation[3] = { rot.x,rot.y,rot.z };
		float	matrixScale[3] = { scale.x,scale.y,scale.z };

		ImGuizmo::DecomposeMatrixToComponents(*objectMatrix->m, matrixTranslation, matrixRotation, matrixScale);
		ImGui::InputFloat3("Tr", matrixTranslation);
		ImGui::InputFloat3("Rt", matrixRotation);
		ImGui::InputFloat3("Sc", matrixScale);
		ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, *objectMatrix->m);

		m_selectedObject->GetTransform()->m_position = D3DXVECTOR3(matrixTranslation[0], matrixTranslation[1], matrixTranslation[2]);
	}

	ImVec2 imgSize;
	ImVec2 imgPos;

	ImGui::Begin("Scene");
	// ImGui::Begin("Scene", 0, gizmoWindowFlags);
	imgSize = ImGui::GetContentRegionAvail();
	imgPos = ImGui::GetCursorScreenPos();
	// シーン画面を画像としてレンダリング
	ImGui::Image((void*)Renderer::GetGameShaderResourceView(), imgSize);

	// オブジェクトを選択していたならマニピュレーターを表示
	if (m_selectedObject) {
		ImGuizmo::SetDrawlist();
		ImGuiIO& io = ImGui::GetIO();
		// ImGuizmo::SetRect(imgPos.x, imgPos.y, imgPos.x+imgSize.x, imgPos.y + imgSize.y);
		// ImGuizmo::SetRect(0.0f, 0.0f, io.DisplaySize.x, io.DisplaySize.y);
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		//ImGuiWindow* window = ImGui::GetCurrentWindow();
		//gizmoWindowFlags = ImGui::IsWindowHovered() && ImGui::IsMouseHoveringRect(window->InnerRect.Min, window->InnerRect.Max) ? ImGuiWindowFlags_NoMove : 0;

		ImGuizmo::Manipulate(*viewMatrix->m, *projectionMatrix->m, currentGuizmoOperation, currentGuizmoMode, *objectMatrix->m, NULL, NULL);
		m_selectedObject->GetTransform()->SetWorldMatrix(objectMatrix);
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
