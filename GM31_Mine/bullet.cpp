#include "main.h"
#include "renderer.h"
#include "bullet.h"
#include "BulletFactory.h"
#include "model.h"
#include "shader.h"

void Bullet::Init(D3DXVECTOR3 direction)
{
	m_Direction = direction;
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	if(m_pModel)
		AddComponent<Model>()->Init(m_pModel);
	m_Transform->m_Scale=D3DXVECTOR3(0.3f, 0.3f, 0.3f);
}


void Bullet::Update()
{
	m_Transform->m_Position = m_Direction;
	// ó£ÇÍÇΩÇÁè¡ãé
}

