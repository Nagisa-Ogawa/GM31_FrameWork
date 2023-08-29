#include "main.h"
#include "renderer.h"
#include "wall.h"
#include "plane.h"
#include "shader.h"
#include "quadCollision.h"

void Wall::Init()
{
	m_Transform->m_Position = D3DXVECTOR3(-4.0f, 0.0f, 0.0f);
	m_Transform->m_Rotation = D3DXVECTOR3(0.0f, 0.0f, -(3.14f / 2.0f));
	AddComponent<Shader>()->Init("Shader\\unlitTextureVS.cso", "Shader\\unlitTexturePS.cso");
	AddComponent<Plane>()->Init(m_Transform->m_Position, D3DXVECTOR2(5.0f, 5.0f), "asset/texture/field004.jpg");
	AddComponent<QuadCollision>()->Init(m_Transform->GetUp(), D3DXVECTOR2(5.0f, 5.0f), false);
}

void Wall::Uninit()
{
	GameObject::Uninit();
}

void Wall::Update()
{
	GameObject::Update();
}

void Wall::Draw()
{
	GameObject::Draw();
}
