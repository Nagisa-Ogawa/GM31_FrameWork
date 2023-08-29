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
	if (m_NowFrame < m_ThrowFrame) {
		D3DXVECTOR3 pos = Throw();
		m_Transform->m_Position = pos;
		m_NowFrame++;
	}

	// m_Transform->m_Position +=m_Direction * m_Speed;
	// 離れたら消去
	//D3DXVECTOR3 vec = m_StartPos - m_Transform->m_Position;
	//float len = D3DXVec3Length(&vec);
	//if (len >= 20.0f) {
	//	BulletFactory::GetInstance()->HideObject(this);
	//}
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
	m_StartPos = m_Transform->m_Position;
	m_EndPos = m_Transform->m_Position + (D3DXVECTOR3(m_pVec.x, 0.0f, m_pVec.z) * 20.0f);
	m_EndPos.y = 0.0f;
	m_StartVec = D3DXVECTOR3(m_pVec.x*3.0f,8.0f,m_pVec.z*3.0f);
	m_EndVec = -m_StartVec;
	m_ThrowFrame = 120.0f;
	m_NowFrame = 0;
}