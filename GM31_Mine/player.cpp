
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


void Player::Init()
{
	m_Transform->m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_Transform->m_Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<BoxCollision>()->Init(m_Transform->m_Scale, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\box.obj");
}

void Player::Update()
{
	D3DXVECTOR3 beforPos = m_Transform->m_Position;
	// ŽOlÌ—p
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
		m_Transform->m_Position += m_Transform->GetForward()*0.1f;
	}
	if (Input::GetKeyPress('Q'))
	{
		m_Transform->m_Rotation.y -= 0.05f;
	}
	if (Input::GetKeyPress('E'))
	{
		m_Transform->m_Rotation.y += 0.05f;
	}

	if (Input::GetKeyTrigger(VK_SPACE)) {
		Bullet* pBullet = BulletFactory::GetInstance()->ActiveObject();
		pBullet->SetDirection(m_Transform->GetForward());
		pBullet->GetTransform()->m_Position = m_Transform->m_Position;
		pBullet->GetTransform()->m_Position.y += 0.5f;
		pBullet->SetPlayerVec(m_Transform->GetForward());
		pBullet->Set();
	}

	bool isHit = false;
	BoxCollision* pPCollision = GetComponent<BoxCollision>();
	auto pEnemies = Manager::GetInstance()->GetScene()->GetGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		BoxCollision* pECollision = pEnemy->GetComponent<BoxCollision>();
		if (CollisionManager::GetInstance()->Collision_BoxToBox(pPCollision, pECollision)) {
			isHit = true;
		}
	}
	if (isHit) {
		m_Transform->m_Position = beforPos;
	}

	isHit = false;

	QuadCollision* qColl = Manager::GetInstance()->GetScene()->GetGameObject<Wall>()->GetComponent<QuadCollision>();
	float l = 0.0f;
	D3DXVECTOR3 dir{};
	if (CollisionManager::GetInstance()->Collision_BoxToQuad(pPCollision, qColl,&l,&dir)) {
		isHit = true;
	}
	if (isHit) {
		m_Transform->m_Position += dir * l;
		// m_IsGround = true;
	}

	//if (m_IsGround) {

	//}
	//else {
	//	m_Transform->m_Position += D3DXVECTOR3(0.0f, -0.02f, 0.0f);
	//}

}

