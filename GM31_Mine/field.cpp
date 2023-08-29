
#include "main.h"
#include "renderer.h"
#include "field.h"
#include "plane.h"
#include "shader.h"

void Field::Init()
{
	D3DXVECTOR3 basePos = D3DXVECTOR3((5 * 10.0f)/2.0f, 0.0f, (5 * 10.0f)/2.0f);
	AddComponent<Shader>()->Init("Shader\\vertexLightingVS.cso", "Shader\\vertexLightingPS.cso");
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			AddComponent<Plane>()->Init(D3DXVECTOR3(basePos.x-x*20.0f, 0.0f, basePos.z-y*20.0f), D3DXVECTOR2(10.0f, 10.0f), "asset/texture/field004.jpg");
		}
	}
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