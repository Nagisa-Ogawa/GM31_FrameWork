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
	// �V�[���p�E�B���h�E���ŉE�N���b�N�������Ƃ����m
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

	auto camera = Manager::GetInstance()->GetScene()->GetGameObject<CameraObject>()->GetComponent<Camera>();
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
