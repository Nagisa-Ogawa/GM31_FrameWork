#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraObject.h"
#include "player.h"


void CameraObject::Init()
{
	m_transform->m_position = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_transform->m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<Camera>()->Init(m_transform->m_position);
}

void CameraObject::Update()
{
	// カメラの操作
	Camera* camera = GetComponent<Camera>();
	Player* player = Manager::GetInstance()->GetScene()->GetGameObject<Player>();

	D3DXVECTOR3 offset = (player->GetTransform()->GetForward()*-5.0f) + D3DXVECTOR3(0.0f, 3.0f, 0.0f);
	camera->SetPosition(player->GetTransform()->m_position + offset);
	camera->SetRotation(player->GetTransform()->m_rotation + D3DXVECTOR3(0.2f, 0.0f, 0.0f));
}


