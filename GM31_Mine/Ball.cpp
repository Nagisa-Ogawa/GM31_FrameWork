#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "Ball.h"
#include "shader.h"
#include "model.h"

#define MAX_WIDTH (30.0f)
#define MAX_HEIGHT (20.0f)
#define MIN_WIDTH (-30.0f)
#define MIN_HEIGHT (-20.0f)

void Ball::Init(MODEL* pModel,D3DXVECTOR3 pos,D3DXVECTOR2 vel,float m)
{
	m_pModel = pModel;
	// 乱数で座標と速度を設定
	m_transform->m_position = pos;
	m_transform->m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_velocity = vel;
	m_m = m;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	if (m_pModel)
		AddComponent<Model>()->Init(m_pModel);

}

void Ball::Update()
{
	// 範囲外チェック
	if (m_transform->m_position.x<MIN_WIDTH || m_transform->m_position.x>MAX_WIDTH) {
		m_velocity.x *= -1.0f;
	}
	if (m_transform->m_position.z<MIN_HEIGHT || m_transform->m_position.z>MAX_HEIGHT) {
		m_velocity.y *= -1.0f;
	}
	m_transform->m_position.x += m_velocity.x;
	m_transform->m_position.z += m_velocity.y;
	m_transform->m_rotation.y += m_velocity.x / 6.0f;
	GameObject::Update();
}
