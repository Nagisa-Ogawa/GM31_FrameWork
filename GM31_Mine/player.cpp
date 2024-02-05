
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "player.h"
#include "model.h"
#include "shader.h"
#include "enemy.h"
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
#include "model.h"

void Player::Init()
{
	m_transform->m_localScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<BoxCollision>()->Init(D3DXVECTOR3(0.5f, 1.0f, 0.5f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
	// AddComponent<SphereCollision>()->Init(1.0f, D3DXVECTOR3(0.0f, 1.0f, 0.0f), false);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\Bot.obj",true);
	//m_Model = AddComponent<AnimationModel>();
	//m_Model->Init("asset\\model\\Bot.fbx");
	//m_Model->LoadAnimation("asset\\model\\Bot_Run.fbx", "Run");
	//m_Model->LoadAnimation("asset\\model\\Bot_Idle.fbx", "Idle");

	//m_AnimationName = "Idle";
	//m_NextAnimationName = "Idle";

	AddComponent<Script>()->Init("testA.lua");
}

void Player::Update()
{
	D3DXVECTOR3 beforPos = m_transform->m_localPosition;
	//bool move = false;
	//if (!move) {
	//	if (m_NextAnimationName != "Idle") {
	//		m_AnimationName = m_NextAnimationName;
	//		m_NextAnimationName = "Idle";
	//		m_BlendRate = 0.0f;
	//	}
	//}

	//m_Model->Update(m_AnimationName.c_str(), m_Time, m_NextAnimationName.c_str(), m_Time, m_BlendRate);
	//m_Time++;
	//if(m_BlendRate<1.0f)
	//	m_BlendRate += 0.1f;

	float groundHeight;
	auto meshField = Manager::GetInstance()->GetScene()->GetGameObject<MeshField>();
	groundHeight = meshField->GetHeight(m_transform->m_localPosition);

	m_transform->m_localPosition.y = groundHeight;

	//---------------------------------
	// 仮
	// --------------------------------
	// プレイヤーと敵（正方形）の当たり判定
	m_IsHitEnemy = false;
	BoxCollision* pPCollision = GetComponent<BoxCollision>();
	auto pEnemies = Manager::GetInstance()->GetScene()->GetActiveGameObjects<Enemy>();
	for (auto pEnemy : pEnemies) {
		BoxCollision* pECollision = pEnemy->GetComponent<BoxCollision>();
		float l = 0.0f;
		D3DXVECTOR3 dir{};
		if (CollisionManager::GetInstance()->Collision_BoxToBox(pPCollision, pECollision,&l,&dir)) {
			m_IsHitEnemy = true;
		}
		if (m_IsHitEnemy) {
			m_transform->m_localPosition += dir * l;
		}
	}

	m_IsHitWall = false;

	// プレイヤーと壁の当たり判定
	//auto pWalls = Manager::GetInstance()->GetScene()->GetGameObjects<Wall>();
	//for (auto pWall : pWalls) {
	//	float l = 0.0f;
	//	D3DXVECTOR3 dir{};
	//	if (CollisionManager::GetInstance()->Collision_BoxToQuad(pPCollision, pWall->GetComponent<QuadCollision>(), &l, &dir)) {
	//		m_IsHitWall = true;
	//	}
	//	if (m_IsHitWall) {
	//		m_transform->m_localPosition += dir * l;
	//	}
	//}

	GameObject::Update();

}

