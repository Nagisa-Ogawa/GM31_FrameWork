#include "main.h"
#include "renderer.h"
#include "shader.h"
#include "model.h"
#include "enemy.h"
#include "boxCollision.h"

void Enemy::Init(MODEL* pModel)
{
	m_pModel = pModel;
	AddComponent<BoxCollision>()->Init(m_Transform->m_Scale, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	if (m_pModel)
		AddComponent<Model>()->Init(m_pModel);
	m_Transform->m_Scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}


void Enemy::Update()
{
}

