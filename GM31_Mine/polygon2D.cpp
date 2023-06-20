
#include "main.h"
#include "polygon2D.h"
#include "shader.h"
#include "sprite.h"


void Polygon2D::Init()
{

	AddComponent<Shader>()->Init("Shader/unlitTextureVS.cso", "Shader/unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(D3DXVECTOR2(200.0f,200.0f), D3DXVECTOR2(200.0f, 200.0f), "asset/texture/Image20230424112827.jpg");
}

void Polygon2D::Uninit() 
{
	GameObject::Uninit();
}

void Polygon2D::Update()
{
	GameObject::Update();
}


void Polygon2D::Draw()
{
	GameObject::Draw();
}