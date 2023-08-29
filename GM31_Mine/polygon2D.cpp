
#include "main.h"
#include "polygon2D.h"
#include "shader.h"
#include "sprite.h"


void Polygon2D::Init()
{

	AddComponent<Shader>()->Init("Shader/unlitTextureVS.cso", "Shader/unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(D3DXVECTOR2(121.0f,SCREEN_HEIGHT-54.0f), D3DXVECTOR2(242.0f, 108.0f), "asset/texture/gr32logo.jpg");
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