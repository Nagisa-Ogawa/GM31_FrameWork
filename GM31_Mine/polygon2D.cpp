
#include "main.h"
#include "polygon2D.h"
#include "shader.h"
#include "sprite.h"


void Polygon2D::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size, const char* textureName)
{
	AddComponent<Shader>()->Init("Assets\\Shaders\\unlitTextureVS.cso", "Assets\\Shaders\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(pos, size, textureName);
}

void Polygon2D::Init(D3DXVECTOR2 pos, D3DXVECTOR2 size)
{
	AddComponent<Shader>()->Init("Assets\\Shaders\\unlitTextureVS.cso", "Assets\\Shaders\\unlitTexturePS.cso");
	AddComponent<Sprite>()->Init(pos, size);
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