#include "main.h"
#include "renderer.h"
#include "bullet.h"
#include "model.h"
#include "shader.h"

void Bullet::Init()
{
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Model>()->Init("asset\\model\\torus.obj");
	m_Transform->m_Scale=D3DXVECTOR3(0.3f, 0.3f, 0.3f);
}


void Bullet::Update()
{
	// ó£ÇÍÇΩÇÁè¡ãé
}

