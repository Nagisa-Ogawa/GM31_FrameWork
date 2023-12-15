
#include "main.h"
#include "renderer.h"
#include "field.h"
#include "plane.h"
#include "shader.h"

void Field::Init()
{
	D3DXVECTOR3 basePos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	AddComponent<Plane>()->Init(basePos, D3DXVECTOR2(20.0f, 20.0f), "asset/texture/floor01.jpg");
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