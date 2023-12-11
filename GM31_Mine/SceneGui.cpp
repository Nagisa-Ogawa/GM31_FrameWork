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


	// �I�u�W�F�N�g��I�����Ă����Ȃ�}�j�s�����[�^�[��\��
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
	// �V�[����ʂ��摜�Ƃ��ă����_�����O
	ImGui::Image((void*)Renderer::GetEditorShaderResourceView(), imgSize);

	// �I�u�W�F�N�g��I�����Ă����Ȃ�}�j�s�����[�^�[��\��
	if (m_selectedObject) {
		Camera* camera;
		D3DXMATRIX* viewMatrix;
		D3DXMATRIX* projectionMatrix;
		D3DXMATRIX objectMatrix;
		D3DXMatrixIdentity(&objectMatrix);

		// �J�����̃r���[�s��ƃv���W�F�N�V�����s����擾
		camera = Manager::GetInstance()->GetEditor()->GetGameObject<CameraObject>()->GetComponent<Camera>();
		viewMatrix = camera->GetViewMatrix();
		projectionMatrix = camera->GetProjectionMatrix();
		// �I�u�W�F�N�g��transform�̏�񂩂�s����쐬
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

		// �}�j�s�����[�^�[�𐶐�
		ImGuizmo::Manipulate(*viewMatrix->m, *projectionMatrix->m, currentGuizmoOperation, currentGuizmoMode, &objectMatrix.m[0][0], NULL, NULL);
		// �}�j�s�����[�^�[���g�p����Ă����Ȃ�
		if (ImGuizmo::IsUsing()) {
			// �}�j�s�����[�^�[�ŕύX���ꂽtransform�����擾
			ImGuizmo::DecomposeMatrixToComponents(&objectMatrix.m[0][0], transArray, rotArray, scaleArray);

			// transform�֑��M
			m_selectedObject->GetTransform()->m_position = D3DXVECTOR3(transArray[0], transArray[1], transArray[2]);
			m_selectedObject->GetTransform()->SetRotationFromDegree(D3DXVECTOR3(rotArray[0], rotArray[1], rotArray[2]));
			m_selectedObject->GetTransform()->m_scale = D3DXVECTOR3(scaleArray[0], scaleArray[1], scaleArray[2]);
		}
	}

	ImGui::End();

	ImVec2 mousePos;
	mousePos.x = 0;
	mousePos.y = 0;
	// �V�[���p�E�B���h�E���ŉE�N���b�N�������Ƃ����m
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
/// �ʏ�̉�ʂŎ擾�������W���Q�[����ʂ������_�����O���Ă����ʂł̍��W�ɕϊ�
/// </summary>
/// <param name="pos">�Q�[���摜�̍����(0,0)�Ƃ����Ƃ��̍��W</param>
/// <param name="imgSize">�摜�̃T�C�Y</param>
/// <returns>�ϊ���̍��W</returns>
POINT SceneGui::ScreenToGameScreenPoint(ImVec2 pos, ImVec2 imgPos, ImVec2 imgSize)
{
	POINT mousePos;
	// �E�B���h�E�̍����(0,0)�Ƃ������W�ɕϊ�
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
/// �}�E�X���W�ɃI�u�W�F�N�g�����邩�𒲂ׂ��̃I�u�W�F�N�g��Ԃ��֐�
/// </summary>
/// <param name="mousePos">�}�E�X���W</param>
/// <returns>���̍��W�ɂ������I�u�W�F�N�g</returns>
GameObject* SceneGui::GetMousePosObject(POINT mousePos)
{
	GameObject* obj = nullptr;
	float minT = -10.0f;
	auto colls = CollisionManager::GetInstance()->GetBoxCollList();

	auto camera = Manager::GetInstance()->GetEditor()->GetGameObject<CameraObject>()->GetComponent<Camera>();
	// �Q�[�����̃Q�[���I�u�W�F�N�g�̐��������[�v
	for (auto coll : colls) {
		D3DXVECTOR3 world1, world2;
		// ���C�𔻒�����I�u�W�F�N�g�̃��[�J�����W�n�ɕϊ�
		auto worldMatrix = coll->GetWorldMatrix();
		CollisionManager::GetInstance()->ScreenToLocalPosition(&worldMatrix,
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 0.0f, &world1);
		CollisionManager::GetInstance()->ScreenToLocalPosition(&worldMatrix,
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 1.0f, &world2);
		// ���C���쐬
		D3DXVECTOR3 vec = world2 - world1;
		D3DXVec3Normalize(&vec, &vec);
		Ray ray(world1, vec);
		auto box = coll;
		float t = -1.0f;
		// ���C�Ƌ��̂œ����蔻��
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
