#include "main.h"
#include "manager.h"
#include "scene.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "bulletFactory.h"
#include "bullet.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "Explosion.h"
#include "enemy.h"
#include "score.h"


void Bullet::Init(MODEL* pModel)
{
	m_pModel = pModel;
	m_Speed = 0.1f;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	if(m_pModel)
		AddComponent<Model>()->Init(m_pModel);
	m_transform->m_worldScale=D3DXVECTOR3(0.3f, 0.3f, 0.3f);
	AddComponent<BoxCollision>()->Init(D3DXVECTOR3(0.6f,0.6f,0.6f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
}


void Bullet::Update()
{
	if (m_NowFrame < m_ThrowFrame) {
		D3DXVECTOR3 pos = Throw();
		m_transform->m_worldPosition = pos;
		m_NowFrame++;
	}

	// 弾と敵の当たり判定
	BoxCollision* pPCollision = GetComponent<BoxCollision>();
	auto pEnemies = Manager::GetInstance()->GetScene()->GetGameObjects<Enemy>();
	for (Enemy* pEnemy : pEnemies) {
		bool isHit = false;
		BoxCollision* pECollision = pEnemy->GetComponent<BoxCollision>();
		if (CollisionManager::GetInstance()->Collision_BoxToBox(pPCollision, pECollision,NULL,NULL)) {
			isHit = true;
		}
		if (isHit) {
			std::string name = m_name + "Explosion";
			Manager::GetInstance()->GetScene()->AddGameObject<Explosion>(1,name)->SetTransform(pEnemy->GetTransform());
			SetActive(false);
			pEnemy->SetActive(false);
			Manager::GetInstance()->GetScene()->GetGameObject<Score>()->AddCount(1);

		}
	}


}

D3DXVECTOR3 Bullet::Throw() {
	D3DXVECTOR3 pos;
	double t = (double)m_NowFrame / (double)m_ThrowFrame;	// 現在の経過割合
	// エルミート曲線を作成
	pos.x = (2.0f * m_StartPos.x + (-2.0f) * m_EndPos.x + m_StartVec.x + m_EndVec.x) * (t * t * t) +
		((-3.0f) * m_StartPos.x + 3.0f * m_EndPos.x + (-2.0f) * m_StartVec.x + (-1.0f) * m_EndVec.x) * (t * t) +
		(m_StartVec.x * t) + m_StartPos.x;
	pos.y = (2.0f * m_StartPos.y + (-2.0f) * m_EndPos.y + m_StartVec.y + m_EndVec.y) * (t * t * t) +
		((-3.0f) * m_StartPos.y + 3.0f * m_EndPos.y + (-2.0f) * m_StartVec.y + (-1.0f) * m_EndVec.y) * (t * t) +
		(m_StartVec.y * t) + m_StartPos.y;
	pos.z = (2.0f * m_StartPos.z + (-2.0f) * m_EndPos.z + m_StartVec.z + m_EndVec.z) * (t * t * t) +
		((-3.0f) * m_StartPos.z + 3.0f * m_EndPos.z + (-2.0f) * m_StartVec.z + (-1.0f) * m_EndVec.z) * (t * t) +
		(m_StartVec.z * t) + m_StartPos.z;

	return pos;
}

void Bullet::Set() {
	m_StartPos = m_transform->m_worldPosition;
	m_EndPos = m_transform->m_worldPosition + (D3DXVECTOR3(m_pVec.x, 0.0f, m_pVec.z) * 20.0f);
	m_EndPos.y = 0.0f;
	m_StartVec = D3DXVECTOR3(m_pVec.x*3.0f,8.0f,m_pVec.z*3.0f);
	m_EndVec = -m_StartVec;
	m_ThrowFrame = 120.0f;
	m_NowFrame = 0;
}