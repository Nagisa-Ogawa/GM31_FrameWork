#include "main.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "bulletFactory.h"
#include "bullet.h"


void Bullet::Init(MODEL* pModel)
{
	m_pModel = pModel;
	m_Speed = 0.1f;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	if(m_pModel)
		AddComponent<Model>()->Init(m_pModel);
	m_Transform->m_Scale=D3DXVECTOR3(0.3f, 0.3f, 0.3f);
}


void Bullet::Update()
{
	m_Transform->m_Position +=m_Direction * m_Speed;

	// —£‚ê‚½‚çÁ‹Ž
	D3DXVECTOR3 vec = m_StartPos - m_Transform->m_Position;
	float len = D3DXVec3Length(&vec);
	if (len >= 20.0f) {
		BulletFactory::GetInstance()->HideBullet(this);
	}
}
