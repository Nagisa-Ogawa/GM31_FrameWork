#include "main.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "enemy.h"
#include "boxCollision.h"

void Enemy::Init(MODEL* pModel)
{
	m_pModel = pModel;
	m_transform->m_localScale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	AddComponent<BoxCollision>()->Init(m_transform->m_localScale, D3DXVECTOR3(0.0f, 3.0f, 0.0f), false);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	if (m_pModel)
		AddComponent<Model>()->Init(m_pModel);
}


void Enemy::Update()
{
	GameObject::Update();
}

