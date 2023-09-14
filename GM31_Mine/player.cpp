
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "input.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"
#include "bulletFactory.h"
#include "bullet.h"
#include "enemy.h"
#include "wall.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "quadCollision.h"
#include "animationModel.h"
#include "camera.h"
#include "cameraObject.h"
#include "Ray.h"
#include "sphereCollision.h"
#include "sphereCollisionFrame.h"
#include "boxCollisionFrame.h"

void Player::Init()
{
	m_Transform->m_Scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_Transform->m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<BoxCollision>()->Init(D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
	// AddComponent<SphereCollision>()->Init(1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Init("asset\\model\\Bot.fbx");
	m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx", "Idle");

	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";
}

void Player::Update()
{
	D3DXVECTOR3 beforPos = m_Transform->m_Position;
	bool move = false;
	// 三人称用
	if (Input::GetKeyPress('A'))
	{
		m_Transform->m_Position -= m_Transform->GetRight()*0.1f;
	}
	if (Input::GetKeyPress('D'))
	{
		m_Transform->m_Position += m_Transform->GetRight()*0.1f;
	}
	if (Input::GetKeyPress('S'))
	{
		m_Transform->m_Position -= m_Transform->GetForward()*0.1f;
	}
	if (Input::GetKeyPress('W'))
	{
		if (m_NextAnimationName != "Run") {
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Run";
			m_BlendRate = 0.0f;
		}

		m_Transform->m_Position += m_Transform->GetForward()*0.1f;
		move = true;
	}
	if (!move) {
		if (m_NextAnimationName != "Idle") {
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;
		}
	}
	if (Input::GetKeyPress('Q'))
	{
		m_Transform->m_Rotation.y -= 0.05f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Transform->m_Rotation.y += 0.05f;
	}

	// 仮-----------------------------
	if (Input::GetKeyPress(VK_LBUTTON)) {
		// スクリーン座標をクライアント座標へ
		auto mousePos = Input::GetClientMousePos();
		D3DXVECTOR3 world1, world2;
		auto worldMatrix = Manager::GetInstance()->GetScene()->GetGameObject<Player>()->
						GetComponent<BoxCollision>()->GetFrame()->GetTransform()->GetWorldMatrix();
		auto camera = Manager::GetInstance()->GetScene()->GetGameObject<CameraObject>()->GetComponent<Camera>();
		CollisionManager::GetInstance()->ScreenToLocalPosition(worldMatrix, 
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 0.0f, &world1);
		CollisionManager::GetInstance()->ScreenToLocalPosition(worldMatrix, 
			camera->GetViewMatrix(), camera->GetProjectionMatrix(), mousePos, 1.0f, &world2);
		// レイを作成
		D3DXVECTOR3 vec = world2 - world1;
		D3DXVec3Normalize(&vec, &vec);
		Ray ray(world1, vec);
		auto box = this->GetComponent<BoxCollision>();
		// レイと球体で当たり判定
		if (CollisionManager::GetInstance()->Collision_RayToBox(&ray, box, NULL, NULL)) {
			Bullet* pBullet = BulletFactory::GetInstance()->ActiveObject();
			pBullet->SetDirection(m_Transform->GetForward());
			pBullet->GetTransform()->m_Position = m_Transform->m_Position;
			pBullet->GetTransform()->m_Position.y += 0.5f;
			pBullet->SetPlayerVec(m_Transform->GetForward());
			pBullet->Set();
		}
	}
	//--------------------------------
	if (Input::GetKeyTrigger('J')) {
		Bullet* pBullet = BulletFactory::GetInstance()->ActiveObject();
		pBullet->SetDirection(m_Transform->GetForward());
		pBullet->GetTransform()->m_Position = m_Transform->m_Position;
		pBullet->GetTransform()->m_Position.y += 0.5f;
		pBullet->SetPlayerVec(m_Transform->GetForward());
		pBullet->Set();
	}

	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);
	m_Time++;
	if(m_BlendRate<1.0f)
		m_BlendRate += 0.1f;

	m_IsHitEnemy = false;
	BoxCollision* pPCollision = GetComponent<BoxCollision>();
	auto pEnemies = Manager::GetInstance()->GetScene()->GetGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		BoxCollision* pECollision = pEnemy->GetComponent<BoxCollision>();
		if (CollisionManager::GetInstance()->Collision_BoxToBox(pPCollision, pECollision)) {
			m_IsHitEnemy = true;
		}
	}
	if (m_IsHitEnemy) {
		m_Transform->m_Position = beforPos;
	}

	m_IsHitWall = false;

	auto pWalls = Manager::GetInstance()->GetScene()->GetGameObjects<Wall>();
	for (Wall* pWall : pWalls) {
		float l = 0.0f;
		D3DXVECTOR3 dir{};
		if (CollisionManager::GetInstance()->Collision_BoxToQuad(pPCollision, pWall->GetComponent<QuadCollision>(), &l, &dir)) {
			m_IsHitWall = true;
		}
		if (m_IsHitWall) {
			m_Transform->m_Position += dir * l;
		}
	}


}

