
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
	GameObject::Draw();
}