
#include "main.h"
#include "renderer.h"
#include "field.h"
#include "plane.h"
#include "shader.h"

void Field::Init()
{
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Plane>()->Init(D3DXVECTOR3(-5.0f, 0.0f, -5.0f), D3DXVECTOR2(10.0f, 10.0f), "asset/texture/field004.jpg");
	AddComponent<Plane>()->Init(D3DXVECTOR3(5.0f, 0.0f, -5.0f), D3DXVECTOR2(10.0f, 10.0f), "asset/texture/field004.jpg");
	AddComponent<Plane>()->Init(D3DXVECTOR3(-5.0f, 0.0f, 5.0f), D3DXVECTOR2(10.0f, 10.0f), "asset/texture/field004.jpg");
	AddComponent<Plane>()->Init(D3DXVECTOR3(5.0f, 0.0f, 5.0f), D3DXVECTOR2(10.0f, 10.0f), "asset/texture/field004.jpg");
}

void Field::Uninit()
{
	GameObject::Uninit();
}

void Field::Update()
{
	GameObject::Update();
}


void Field::Draw()
{
	// マトリクス設定
	D3DXMATRIX world, scale, rot, trans;
	D3DXMatrixScaling(&scale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixRotationYawPitchRoll(&rot, m_Rotation.y, m_Rotation.x, m_Rotation.z);
	D3DXMatrixTranslation(&trans, m_Position.x, m_Position.y, m_Position.z);
	world = scale * rot * trans;
	Renderer::SetWorldMatrix(&world);

	GameObject::Draw();

}