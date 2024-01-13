
#include "main.h"
#include "manager.h"
#include "scene.h"
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
#include "meshField.h"
#include "input.h"
#include "script.h"

void Player::Init()
{
	m_transform->m_scale = D3DXVECTOR3(0.01f, 0.01f, 0.01f);
	m_transform->m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<BoxCollision>()->Init(D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
	// AddComponent<SphereCollision>()->Init(1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	m_Model = AddComponent<AnimationModel>();
	m_Model->Init("asset\\model\\Bot.fbx");
	m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx", "Idle");

	m_AnimationName = "Idle";
	m_NextAnimationName = "Idle";

	AddComponent<Script>()->Init("testA.lua");
}

void Player::Update()
{
	D3DXVECTOR3 beforPos = m_transform->m_position;
	bool move = false;
	// 三人称用
	//if (Input::GetKeyPress('A'))
	//{
	//	m_transform->m_position -= m_transform->GetRight()*0.1f;
	//}
	//if (Input::GetKeyPress('D'))
	//{
	//	m_transform->m_position += m_transform->GetRight()*0.1f;
	//}
	//if (Input::GetKeyPress('S'))
	//{
	//	m_transform->m_position -= m_transform->GetForward()*0.1f;
	//}
	//if (Input::GetKeyPress('W'))
	//{
	//	if (m_NextAnimationName != "Run") {
	//		m_AnimationName = m_NextAnimationName;
	//		m_NextAnimationName = "Run";
	//		m_BlendRate = 0.0f;
	//	}

	//	m_transform->m_position += m_transform->GetForward()*0.1f;
	//	move = true;
	//}
	if (!move) {
		if (m_NextAnimationName != "Idle") {
			m_AnimationName = m_NextAnimationName;
			m_NextAnimationName = "Idle";
			m_BlendRate = 0.0f;
		}
	}

	//if (Input::GetKeyTrigger('J')) {
	//	Bullet* pBullet = BulletFactory::GetInstance()->ActiveObject();
	//	pBullet->SetDirection(m_Transform->GetForward());
	//	pBullet->GetTransform()->m_Position = m_Transform->m_Position;
	//	pBullet->GetTransform()->m_Position.y += 0.5f;
	//	pBullet->SetPlayerVec(m_Transform->GetForward());
	//	pBullet->Set();
	//}

	m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);
	m_Time++;
	if(m_BlendRate<1.0f)
		m_BlendRate += 0.1f;

	float groundHeight;
	auto meshField = Manager::GetInstance()->GetScene()->GetGameObject<MeshField>();
	groundHeight = meshField->GetHeight(m_transform->m_position);

	m_transform->m_position.y = groundHeight;

	//---------------------------------
	// 仮
	// --------------------------------
	// プレイヤーと敵（正方形）の当たり判定
	m_IsHitEnemy = false;
	BoxCollision* pPCollision = GetComponent<BoxCollision>();
	auto pEnemies = Manager::GetInstance()->GetScene()->GetActiveGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		BoxCollision* pECollision = pEnemy->GetComponent<BoxCollision>();
		float l = 0.0f;
		D3DXVECTOR3 dir{};
		if (CollisionManager::GetInstance()->Collision_BoxToBox(pPCollision, pECollision,&l,&dir)) {
			m_IsHitEnemy = true;
		}
		if (m_IsHitEnemy) {
			m_transform->m_position += dir * l;
		}
	}

	m_IsHitWall = false;

	// プレイヤーと壁の当たり判定
	auto pWalls = Manager::GetInstance()->GetScene()->GetGameObjects<Wall>();
	for (Wall* pWall : pWalls) {
		float l = 0.0f;
		D3DXVECTOR3 dir{};
		if (CollisionManager::GetInstance()->Collision_BoxToQuad(pPCollision, pWall->GetComponent<QuadCollision>(), &l, &dir)) {
			m_IsHitWall = true;
		}
		if (m_IsHitWall) {
			m_transform->m_position += dir * l;
		}
	}

	GameObject::Update();

}

